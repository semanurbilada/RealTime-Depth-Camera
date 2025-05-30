#include <vector>
#include <fstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/all_layers.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

int clip(int n, int lower, int upper) {
    return std::max(lower, std::min(n, upper));
}

// Output Names from Neural Network (default)
vector<string> getOutputsNames(const cv::dnn::Net& net)
{
    static vector<string> names;
    if (names.empty()) {
        std::vector<int32_t> out_layers = net.getUnconnectedOutLayers();
        std::vector<string> layers_names = net.getLayerNames();
        names.resize(out_layers.size());
        for (size_t i = 0; i < out_layers.size(); ++i) {
            names[i] = layers_names[out_layers[i] - 1];
        }
    }
    return names;
}

int main() {
    string file_path = "./src/models/";

    // Read Network
    string model = "midas_v21_384.onnx"; // MiDaS v2.1 Large
    // string model = "midas_v21_small_256.onnx"; // MiDaS v2.1 Small

    // Read in the neural network from the files
    auto net = readNet(file_path+model);    

    // If the model is empty, program should be terminate
    if (net.empty())
    {
        return -1;
    }

    // Run on GPU with CUDA support
    // net.setPreferableBackend(DNN_BACKEND_CUDA);
    // net.setPreferableTarget(DNN_TARGET_CUDA);

    // Run on CPU (macOS not supporting GPU with CUDA)
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);

    // Open up the webcam with configs
    VideoCapture cap(1);
    cap.set(CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CAP_PROP_FRAME_HEIGHT, 240);


    // Loop running as long as webcam is open and "q" is not pressed
    while (cap.isOpened()) {

        // Load in an image
        Mat image;
        cap.read(image);

        // If the image is empty, webcam should be terminate
        if (image.empty()) {
            cv::waitKey(0);
            cout << "image not available!" << endl;
            break;
        }

        int image_width = image.rows;
        int image_height = image.cols;

        auto start = getTickCount();

        // Create Blob from Input Image
        // MiDas v2.1 Large Model -> Size : 384 x 384
        Mat blob = cv::dnn::blobFromImage(image, 1 / 255.f, cv::Size(384, 384), cv::Scalar(123.675, 116.28, 103.53), true, false);
        
        // MiDaS v2.1 Small ( Scale : 1 / 255, Size : 256 x 256, Mean Subtraction : ( 123.675, 116.28, 103.53 ), Channels Order : RGB )
        // Mat blob = blobFromImage(image, 1 / 255.f, cv::Size(256, 256), cv::Scalar(123.675, 116.28, 103.53), true, false);

        // Set the blob to be input to the neural network
        net.setInput(blob);

        // Forward pass of the blob through the neural network to get the predictions
        Mat output = net.forward(getOutputsNames(net)[0]);

        // Convert Size to 384x384 from 1x384x384
        const std::vector<int32_t> size = { output.size[1], output.size[2] };
        output = cv::Mat(static_cast<int32_t>(size.size()), &size[0], CV_32F, output.ptr<float>());

        // Resize Output Image to Input Image Size
        cv::resize(output, output, image.size());


        // Visualize Output Image
        double min, max;
        cv::minMaxLoc(output, &min, &max);
        const double range = max - min;

        // 1. Normalize ( 0.0 - 1.0 )
        output.convertTo(output, CV_32F, 1.0 / range, -(min / range));

        // 2. Scaling ( 0 - 255 )
        output.convertTo(output, CV_8U, 255.0);


        auto end = getTickCount();
        auto totalTime = (end - start) / getTickFrequency();
        auto fps = 1 / totalTime;


        // Display FPS and real-time frame
        putText(output, "FPS: " + to_string(int(fps)), Point(50, 50), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 2, false);

        imshow("Original Frames", image);
        imshow("Depth Estimation", output);

        // Press 'q' to exit the program
        if(waitKey(1) == 'q'){
            break;
        }
    }
    
    cap.release();
    destroyAllWindows();
}
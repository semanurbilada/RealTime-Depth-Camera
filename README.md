# Real-time Depth Estimation with OpenCV & Neural Networks in C++

> **AI Lecture Term Project – 4th Grade, 2nd Term**

This project demonstrates real-time monocular depth estimation using a pre-trained MiDaS (v2.1 Large) model in ONNX format. Built with C++ and OpenCV’s DNN module, it captures live webcam feed and visualizes depth maps in real time.

---

## 📁 Project Structure


---

## ⚙️ Requirements

- C++11 or later
- OpenCV 4.x (built with DNN module)
- Webcam (for live video input)
- MiDaS ONNX model (included in `src/models`)

---

## 🚀 Build and Run

You can build and run the project using the provided shell script:

```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

This script will:

- Create the build/ directory if it doesn't exist
- Compile depth_estimate.cpp
- Run the resulting executable
- ❗️Note: Ensure the model file midas_v21_384.onnx is located at src/models/.

---

## 🧠 About MiDaS

MiDaS (Monocular Depth Estimation) is a deep learning model developed by Intel Labs that estimates depth from a single RGB image. This project uses the v2.1 Large model in ONNX format.

---

## 📷 Output

- Displays the original webcam frame
- Displays the corresponding real-time depth map
- Shows the current FPS in the depth window
- Press q to exit the application.

---

## 📌 Notes

- For macOS or systems without GPU support, the model runs on CPU by default.
- You can modify the backend to use CUDA if supported:
```
net.setPreferableBackend(DNN_BACKEND_CUDA);
net.setPreferableTarget(DNN_TARGET_CUDA);
```

---

## 📄 License

This project is for academic use only. Model usage is subject to its original [MiDaS license.](https://github.com/isl-org/MiDaS)

---

## ✍️ Resources

- [Original Document | MiDaS - Getting Started - Models](https://docs.doji-tech.com/com.doji.midas/manual/models.html)
- [YouTube | How to Estimate Depth with a Monocular Camera using OpenCV C++ and Neural Networks](https://www.youtube.com/watch?v=7fCheEYUpgU&t=409s)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// CNN parameters
#define INPUT_SIZE 5       // 5x5 input image
#define FILTER_SIZE 3      // 3x3 convolution filter
#define OUTPUT_SIZE 3      // 3x3 output after convolution
#define FC_NODES 9         // Fully connected layer nodes
#define LEARNING_RATE 0.01 // Learning rate

// Activation function: ReLU
double relu(double x) {
    return (x > 0) ? x : 0;
}

// Derivative of ReLU
double relu_derivative(double x) {
    return (x > 0) ? 1 : 0;
}

// Softmax activation function
void softmax(double *input, double *output, int size) {
    double sum_exp = 0.0;
    for (int i = 0; i < size; i++) {
        output[i] = exp(input[i]);
        sum_exp += output[i];
    }
    for (int i = 0; i < size; i++) {
        output[i] /= sum_exp;
    }
}

// Forward pass: Convolution
void convolution(double input[INPUT_SIZE][INPUT_SIZE], double filter[FILTER_SIZE][FILTER_SIZE], double output[OUTPUT_SIZE][OUTPUT_SIZE]) {
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            double sum = 0;
            for (int fi = 0; fi < FILTER_SIZE; fi++) {
                for (int fj = 0; fj < FILTER_SIZE; fj++) {
                    sum += input[i + fi][j + fj] * filter[fi][fj];
                }
            }
            output[i][j] = relu(sum); // Apply ReLU activation
        }
    }
}

// Backpropagation: Update convolution filter
void update_filter(double input[INPUT_SIZE][INPUT_SIZE], double d_conv[OUTPUT_SIZE][OUTPUT_SIZE], double filter[FILTER_SIZE][FILTER_SIZE]) {
    for (int i = 0; i < FILTER_SIZE; i++) {
        for (int j = 0; j < FILTER_SIZE; j++) {
            double grad = 0;
            for (int x = 0; x < OUTPUT_SIZE; x++) {
                for (int y = 0; y < OUTPUT_SIZE; y++) {
                    grad += input[x + i][y + j] * d_conv[x][y]; // Compute gradient
                }
            }
            filter[i][j] -= LEARNING_RATE * grad; // Update filter weights
        }
    }
}

// Fully Connected Forward Pass
void fully_connected(double conv_out[OUTPUT_SIZE][OUTPUT_SIZE], double weights[FC_NODES], double *output) {
    *output = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            *output += conv_out[i][j] * weights[i * OUTPUT_SIZE + j];
        }
    }
}

// Backpropagation: Update Fully Connected Layer Weights
void update_fc_weights(double conv_out[OUTPUT_SIZE][OUTPUT_SIZE], double *fc_weights, double error) {
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            fc_weights[i * OUTPUT_SIZE + j] -= LEARNING_RATE * error * conv_out[i][j];
        }
    }
}

// Train CNN with one input and one expected output
void train(double input[INPUT_SIZE][INPUT_SIZE], double expected) {
    // Initialize filter and weights randomly
    double filter[FILTER_SIZE][FILTER_SIZE] = {{0.1, -0.2, 0.3}, {-0.4, 0.5, -0.6}, {0.7, -0.8, 0.9}};
    double fc_weights[FC_NODES] = {0.1, -0.1, 0.2, -0.2, 0.3, -0.3, 0.4, -0.4, 0.5};
    
    for (int epoch = 0; epoch < 1000; epoch++) { // Training loop
        double conv_out[OUTPUT_SIZE][OUTPUT_SIZE];
        convolution(input, filter, conv_out); // Forward propagation

        double fc_output;
        fully_connected(conv_out, fc_weights, &fc_output); // Forward FC

        // Compute softmax loss (for a simple binary classification case)
        double softmax_out[2];
        double logits[2] = {fc_output, -fc_output};
        softmax(logits, softmax_out, 2);
        double loss = -log(softmax_out[(int)expected]); // Cross-entropy loss

        // Compute output layer error
        double d_fc_output = softmax_out[0] - (expected == 0 ? 1 : 0);

        // Compute gradients for convolution layer (backpropagation)
        double d_conv[OUTPUT_SIZE][OUTPUT_SIZE];
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            for (int j = 0; j < OUTPUT_SIZE; j++) {
                d_conv[i][j] = d_fc_output * fc_weights[i * OUTPUT_SIZE + j] * relu_derivative(conv_out[i][j]);
            }
        }

        // Update weights
        update_fc_weights(conv_out, fc_weights, d_fc_output);
        update_filter(input, d_conv, filter);

        // Print loss
        if (epoch % 100 == 0) {
            printf("Epoch %d, Loss: %f\n", epoch, loss);
        }
    }
}

int main() {
    double input[INPUT_SIZE][INPUT_SIZE] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };
    
    double expected = 0; // Class label (binary classification example)
    
    train(input, expected); // Train CNN

    return 0;
}
#include "pch.h"
#include "framework.h"
#include "Neuron.h"

#ifndef __MLP_NETWORK_CPP__
#define __MLP_NETWORK_CPP__

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>

#include "MlpNetwork.h"



/*int main() {
    srand(42); // Фиксируем случайный сид

    // Веса нейронной сети
    double W1[HIDDEN1_SIZE][INPUT_SIZE];
    double W2[HIDDEN2_SIZE][HIDDEN1_SIZE];
    double W3[OUTPUT_SIZE][HIDDEN2_SIZE];

    // Инициализация весов
    initialize_weights((double*)W1, HIDDEN1_SIZE * INPUT_SIZE);
    initialize_weights((double*)W2, HIDDEN2_SIZE * HIDDEN1_SIZE);
    initialize_weights((double*)W3, OUTPUT_SIZE * HIDDEN2_SIZE);

    // Пример входного вектора
    double input[INPUT_SIZE] = { 1, 1, 1, 1, 1, ... };  // 10×10 картинка

    // Выходные слои
    double hidden1[HIDDEN1_SIZE];
    double hidden2[HIDDEN2_SIZE];
    double output[OUTPUT_SIZE];

    // Прямое распространение
    forward(input, hidden1, hidden2, output, W1, W2, W3);

    // Вывод результатов
    printf("Выходные значения:\n");
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        printf("Класс %d: %.4f\n", i, output[i]);
    }

    return 0;
}*/


template <class Type>
CMlpNetwork<Type>::CMlpNetwork(int inputSize, int classCount, Type learningRate)
    : m_inputSize(inputSize)
    , m_classCount(classCount)
    , m_learningRate(learningRate)
    , m_Initialized(false)
{

    m_hidden1Count = inputSize / 6;
    m_hidden2Count = m_hidden1Count / 2;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    W1.resize(m_hidden1Count, std::vector<Type>(inputSize));
    W2.resize(m_hidden2Count, std::vector<Type>(m_hidden1Count));
    W3.resize(classCount, std::vector<Type>(m_hidden2Count));

    initializeWeights(W1);
    initializeWeights(W2);
    initializeWeights(W3);
}


template <class Type>
CMlpNetwork<Type>::~CMlpNetwork()
{
    if (!m_Initialized)
        return;
}


template <class Type>
void
CMlpNetwork<Type>::initializeWeights(std::vector<std::vector<Type>>& weights)
{
    for (auto& row : weights)
    {
        for (auto& w : row)
            w = ((Type)std::rand() / RAND_MAX) * 2.0 - 1.0;
    }

    m_Initialized = true;
}

template <class Type>
std::vector<Type>
CMlpNetwork<Type>::softmax(const std::vector<Type>& input) const
{
    std::vector<Type> result(input.size());

    Type maxVal = *std::max_element(input.begin(), input.end());

    Type sum = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        result[i] = std::exp(input[i] - maxVal);
        sum += result[i];
    }

    for (auto& val : result) val /= sum;

    return result;
}


template <class Type>
void
CMlpNetwork<Type>::forward(
    const std::vector<Type>& input,
    std::vector<Type>& hidden1,
    std::vector<Type>& hidden2,
    std::vector<Type>& output
    )
{
    hidden1.assign(m_hidden1Count, 0.0);
    hidden2.assign(m_hidden2Count, 0.0);
    output.assign(m_classCount, 0.0);

    for (int i = 0; i < m_hidden1Count; ++i)
    {
        for (int j = 0; j < m_inputSize; ++j)
            hidden1[i] += W1[i][j] * input[j];
    }

    for (auto& h : hidden1)
        h = sigmoid(h);

    for (int i = 0; i < m_hidden2Count; ++i)
    {
        for (int j = 0; j < m_hidden1Count; ++j)
            hidden2[i] += W2[i][j] * hidden1[j];
    }

    for (auto& h : hidden2)
        h = sigmoid(h);

    for (int i = 0; i < m_classCount; ++i)
    {
        for (int j = 0; j < m_hidden2Count; ++j)
            output[i] += W3[i][j] * hidden2[j];
    }

    for (auto& o : output)
        o = sigmoid(o);
}

template <class Type>
void
CMlpNetwork<Type>::backward(
    const std::vector<Type>& input,
    const std::vector<Type>& hidden1,
    const std::vector<Type>& hidden2,
    const std::vector<Type>& output,
    const std::vector<Type>& target
    )
{

    std::vector<Type> output_error(m_classCount);
    std::vector<Type> hidden2_error(m_hidden2Count);
    std::vector<Type> hidden1_error(m_hidden1Count);

    for (int i = 0; i < m_classCount; ++i)
        output_error[i] = (target[i] - output[i]) * sigmoid_derivative(output[i]);

    for (int i = 0; i < m_hidden2Count; ++i)
    {
        hidden2_error[i] = 0;
        for (int j = 0; j < m_classCount; ++j)
            hidden2_error[i] += output_error[j] * W3[j][i];

        hidden2_error[i] *= sigmoid_derivative(hidden2[i]);
    }

    for (int i = 0; i < m_hidden1Count; ++i)
    {
        hidden1_error[i] = 0;
        for (int j = 0; j < m_hidden2Count; ++j)
            hidden1_error[i] += hidden2_error[j] * W2[j][i];

        hidden1_error[i] *= sigmoid_derivative(hidden1[i]);
    }

    for (int i = 0; i < m_classCount; ++i)
    {
        for (int j = 0; j < m_hidden2Count; ++j)
            W3[i][j] += m_learningRate * output_error[i] * hidden2[j];
    }

    for (int i = 0; i < m_hidden2Count; ++i)
    {
        for (int j = 0; j < m_hidden1Count; ++j)
            W2[i][j] += m_learningRate * hidden2_error[i] * hidden1[j];
    }

    for (int i = 0; i < m_hidden1Count; ++i)
    {
        for (int j = 0; j < m_inputSize; ++j)
            W1[i][j] += m_learningRate * hidden1_error[i] * input[j];
    }
}

template <class Type>
void
CMlpNetwork<Type>::reset()
{

    initializeWeights(W1);
    initializeWeights(W2);
    initializeWeights(W3);

    m_Initialized = true;
}


template <class Type>
void
CMlpNetwork<Type>::trainOneSample(const std::vector<Type>& input, int expectedClass, int maxEpochs, Type minImprovement)
{
    Type prevOutput = 0.0;

    for (int epoch = 0; epoch < maxEpochs; ++epoch)
    {
        std::vector<Type> hidden1(m_hidden1Count);
        std::vector<Type> hidden2(m_hidden2Count);
        std::vector<Type> output(m_classCount);

        forward(input, hidden1, hidden2, output);

        std::vector<Type> target(m_classCount, 0.0);

        target[expectedClass] = 1.0;

        backward(input, hidden1, hidden2, output, target);

        Type delta = std::abs(output[expectedClass] - prevOutput);
        prevOutput = output[expectedClass];

        if (delta < minImprovement)
            break;
    }

}

template <class Type>
std::vector<Type>
CMlpNetwork<Type>::classify(const std::vector<Type>& input)
{
    std::vector<Type> hidden1(m_hidden1Count);
    std::vector<Type> hidden2(m_hidden2Count);
    std::vector<Type> output(m_classCount);

    forward(input, hidden1, hidden2, output);

    /*int bestClass = 0;
    Type bestScore = output[0];


    for (int i = 1; i < m_classCount; ++i)
    {
        if (output[i] > bestScore) {
            bestScore = output[i];
            bestClass = i;
        }
    }

    return { bestClass, bestScore };*/

    return output;
}

template <class Type>
std::pair<int, Type>
CMlpNetwork<Type>::classifyBest(const std::vector<Type>& input)
{
    std::vector<Type> output = classify(input);
    std::vector<Type> conf = softmax(output);
    int best = 0;
    for (int i = 1; i < m_classCount; ++i)
        if (conf[i] > conf[best]) best = i;
    return { best, conf[best] };
}


#endif // __MLP_NETWORK_CPP__

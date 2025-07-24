#pragma once

#ifndef __MLP_NETWORK_H__
#define __MLP_NETWORK_H__

#include "Neuron.h"
#include <vector>

#include <cmath>

template <class Type>
class CMlpNetwork
{

    inline Type sigmoid(Type x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    inline Type sigmoid_derivative(Type y) {
        return y * (1.0 - y);
    }

private:
    int     m_inputSize;
    Type    m_learningRate;
    Type    m_epsilon = 0.01;
    bool    m_Initialized = false;
    int     m_classCount;
    int     m_hidden1Count;
    int     m_hidden2Count;

public:

    CMlpNetwork(int inputSize, int classCount, Type learningRate = 0.1);
    ~CMlpNetwork();

    void reset();


    void trainOneSample(const std::vector<Type>& input, int expectedClass, int maxEpochs = 100, Type minImprovement = 0.0001);

    std::vector<Type> softmax(const std::vector<Type>& input) const;

    std::vector<Type> classify(const std::vector<Type>& input);
    std::pair<int, Type> classifyBest(const std::vector<Type>& input);

private:

    std::vector<std::vector<Type>> W1;
    std::vector<std::vector<Type>> W2;
    std::vector<std::vector<Type>> W3;

    void initializeWeights(std::vector<std::vector<Type>>& weights);

    void forward(const std::vector<Type>& input,
        std::vector<Type>& hidden1,
        std::vector<Type>& hidden2,
        std::vector<Type>& output);

    void backward(const std::vector<Type>& input,
        const std::vector<Type>& hidden1,
        const std::vector<Type>& hidden2,
        const std::vector<Type>& output,
        const std::vector<Type>& target);
};


#include "MlpNetwork.cpp"

#endif // __MLP_NETWORK_H__

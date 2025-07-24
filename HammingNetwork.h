#pragma once

#ifndef __HAMMING_NETWORK_H__
#define __HAMMING_NETWORK_H__

#include "Neuron.h"
#include <vector>

template <class Type>
class CHammingNetwork
{

public:
    CHammingNetwork(int inputSize, int classCount, Type learningRate = 0.05);
    ~CHammingNetwork();


    void reset();

    void train(
        const std::vector<std::vector<Type>>& trainingData,   // samples array
        const std::vector<int>& targetClasses,                // classes
        int epochs);                                          // train epochs

    void trainOneSample(const std::vector<Type>& input, int expectedClass);


    int classify(const std::vector<Type>& input);

    std::vector<Type> forward(const std::vector<Type>& input);

private:
    int     m_inputSize;
    int     m_classCount;
    Type    m_learningRate;
    Type    m_epsilon = 0.01;
    bool    m_Initialized = false;
    const int  m_maxIterations = 10;

    std::vector<std::vector<Type>> W1;
    std::vector<std::vector<Type>> W2;

    Type activation(Type x) const;
    Type activation2(Type x) const;

    void initializeWeights(const std::vector<std::vector<Type>>& trainingData);
    void firstLayer(const std::vector<Type>& input, std::vector<Type>& output);
    void secondLayer(std::vector<Type>& y);
};


#include "HammingNetwork.cpp"

#endif // __HAMMING_NETWORK_H__
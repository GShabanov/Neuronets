#include "pch.h"
#include "framework.h"
#include "Neuron.h"

#ifndef __HAMMING_NETWORK_CPP__
#define __HAMMING_NETWORK_CPP__

#include "HammingNetwork.h"


template <class Type>
CHammingNetwork<Type>::CHammingNetwork(int inputSize, int classCount, Type learningRate)
    : m_inputSize(inputSize)
    , m_classCount(classCount)
    , m_learningRate(learningRate)
    , m_Initialized(false)
{
    //std::vector<Type> a(inputSize);

    W1.resize(classCount, std::vector<Type>(inputSize));
    W2.resize(classCount, std::vector<Type>(classCount));
}


template <class Type>
CHammingNetwork<Type>::~CHammingNetwork()
{
    if (!m_Initialized)
        return;
}


template <class Type>
void
CHammingNetwork<Type>::initializeWeights(const std::vector<std::vector<Type>>& trainingData)
{
    for (int i = 0; i < m_classCount; i++)
    {
        for (int j = 0; j < m_inputSize; j++)
        {
            //W1[i][j] = trainingData[i][j] + ((rand() % 100) / 500.0); // noise
            W1[i][j] = (training_data[i][j] / 127.5) - 1.0;
        }
    }

    Type alpha = 0.2;   // 10

    for (int i = 0; i < m_classCount; i++) {

        for (int j = 0; j < m_classCount; j++) {

            W2[i][j] = (i == j) ? 1.0 : -alpha;
        }
    }

    m_Initialized = true;

}

template <class Type>
Type
CHammingNetwork<Type>::activation(Type x) const
{
    return x; // linear 
}

template <class Type>
Type
CHammingNetwork<Type>::activation2(Type x) const
{
    //return x / (1 + std::abs(x));  // range (-1..+1)
    if (x <= 0.0)
        return 0.0;
    if (x >= 20000.0) 
        return 20000.0;
    else
        return x;
}

template <class Type>
void
CHammingNetwork<Type>::firstLayer(
    const std::vector<Type>& input,
    std::vector<Type>& output
    )
{
    output.resize(m_classCount);

    for (int i = 0; i < m_classCount; i++)
    {
        Type sum = 0;

        for (int j = 0; j < m_inputSize; j++)
        {
            sum += W1[i][j] * input[j];
        }

        output[i] = activation(sum);
    }
}

template <class Type>
void
CHammingNetwork<Type>::secondLayer(std::vector<Type>& y)
{
    std::vector<Type> y_new(m_classCount);
    std::vector<Type> y_old = y;

    const Type E = 1.0 / m_classCount;

    bool converged = false;
    int iter = 0;

    while (!converged && iter < m_maxIterations)
    {
        for (int i = 0; i < m_classCount; ++i)
        {
            Type inhibition = 0;

            for (int j = 0; j < m_classCount; ++j)
            {
                if (i != j)
                    inhibition += y_old[j];
            }

            y_new[i] = y_old[i] - E * inhibition;
            y_new[i] = activation2(y_new[i]); // классическая Ramp
        }

        // проверка сходимости
        Type diff = 0;

        for (int i = 0; i < m_classCount; ++i)
            diff += std::abs(y_new[i] - y_old[i]);

        if (diff < m_epsilon)
            converged = true;

        y_old = y_new;
        ++iter;
    }

    y = y_new;
}


template <class Type>
void
CHammingNetwork<Type>::reset()
{
    W1.resize(m_classCount, std::vector<Type>(m_inputSize, 0));
    W2.resize(m_classCount, std::vector<Type>(m_classCount, 0));

    for (int i = 0; i < m_classCount; ++i)
    {
        for (int j = 0; j < m_classCount; ++j)
        {
            if (i == j)
                W2[i][j] = 1.0;
            else
                W2[i][j] = -1.0 / static_cast<Type>(m_classCount); // классическое подавление
        }
    }

    m_Initialized = true;
}

template <class Type>
void
CHammingNetwork<Type>::train(
    const std::vector<std::vector<Type>>& trainingData,
    const std::vector<int>& targetClasses,
    int epochs)
{

    initializeWeights(trainingData);

    for (int epoch = 0; epoch < epochs; epoch++) {

        //std::cout << "Эпоха " << epoch + 1 << "\n";

        for (int i = 0; i < trainingData.size(); i++)
        {

            std::vector<Type> y;

            firstLayer(trainingData[i], y);
            secondLayer(y);

            int expected = targetClasses[i];

            // renew for W1
            for (int j = 0; j < m_inputSize; j++)
            {

                W1[expected][j] += learningRate * (trainingData[i][j] - W1[expected][j]);
            }

            // renew for W2

            for (int j = 0; j < m_classCount; j++)
            {
                if (j == expected)

                    W2[expected][j] = 1.0;
                else
                    W2[expected][j] -= learningRate * std::abs(y[j]);
            }
        }
    }
}

template <class Type>
void
CHammingNetwork<Type>::trainOneSample(const std::vector<Type>& input, int expectedClass)
{
    for (int j = 0; j < m_inputSize; ++j)
    {
        W1[expectedClass][j] = input[j] / 2.0; // normalised (-0.5 .. +0.5)
    }
}

template <class Type>
std::vector<Type>
CHammingNetwork<Type>::forward(const std::vector<Type>& input)
{
    std::vector<Type> y;
    firstLayer(input, y);
    secondLayer(y);

    return y;
}

template <class Type>
int
CHammingNetwork<Type>::classify(const std::vector<Type>& input)
{

    std::vector<Type> y = forward(input);

    int best = 0;

    for (int i = 1; i < y.size(); i++) {
        if (y[i] > y[best]) best = i;
    }

    return best;
}



#endif // __HAMMING_NETWORK_CPP__

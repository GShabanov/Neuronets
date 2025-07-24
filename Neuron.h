#pragma once

#ifndef __NEURON_H__
#define __NEURON_H__


//
// нейрон
//
template <class DataType>
class  CNeuron
{
    DataType  m_threshold;
    DataType  m_value;


public:

    CNeuron(DataType initialValue)
    {
        m_threshold = 0.5;
        m_value = initialValue;
    }

};

template <class DataType>
class Neuron
{
public:
    DataType    *Weights;       // Weights for this neuron
    DataType     Y;             // выход нейрона (активаци€)             
    DataType     S;             // сумма входов (будет считатьс€ в первом слое)
};

template <class DataType>
class NeuralNetwork
{
public:
    int         n;      // n Ч input vector size (200x200 = 40000 pixels)
    int         m;      // m Ч classes count
    DataType    T;      // gate constant
    DataType    E;      //  оэффициент подавлени€ (E = 1/m Ч классическое значение из теории ’емминга)
    DataType    Emax;   //  ритерий остановки итераций во втором слое (точность сходимости)
    Neuron<DataType>* FirstLayer;
    Neuron<DataType>* SecondLayer;
};


template <class Type>
class CNetworkUpdateCallback
{
public:
    virtual void NetworkUpdateCallback(const CArray<Type>& output) = 0;
};



#endif // __NEURON_H__
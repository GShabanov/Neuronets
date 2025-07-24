#pragma once

#ifndef __NEURON_H__
#define __NEURON_H__


//
// ������
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
    DataType     Y;             // ����� ������� (���������)             
    DataType     S;             // ����� ������ (����� ��������� � ������ ����)
};

template <class DataType>
class NeuralNetwork
{
public:
    int         n;      // n � input vector size (200x200 = 40000 pixels)
    int         m;      // m � classes count
    DataType    T;      // gate constant
    DataType    E;      // ����������� ���������� (E = 1/m � ������������ �������� �� ������ ��������)
    DataType    Emax;   // �������� ��������� �������� �� ������ ���� (�������� ����������)
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
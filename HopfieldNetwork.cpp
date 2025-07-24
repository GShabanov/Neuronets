#include "pch.h"
#include "framework.h"
#include "Neuron.h"

#ifndef __HOPFIELD_NETWORK_CPP__
#define __HOPFIELD_NETWORK_CPP__
#include "HopfieldNetwork.h"

template <class Type>
CHopfieldNetwork<Type>::CHopfieldNetwork(size_t effectorsCount)
{
    m_InputsCount = effectorsCount;
    m_Initialized = false;
    m_Trained = false;
}


template <class Type>
CHopfieldNetwork<Type>::~CHopfieldNetwork()
{
    if (!m_Initialized)
        return;


    //
    // видаляємо нейрони
    //
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        delete m_Neurons[i];
    }

    m_Neurons.RemoveAll();
}


template <class Type>
void
CHopfieldNetwork<Type>::Reset()
{
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        for (SIZE_T j = 0; j < m_InputsCount; j++)
        {
            m_weightMatrix[i * m_InputsCount + j] = 0.0;
        }
    }


    m_Initialized = true;
}

template <class Type>
bool
CHopfieldNetwork<Type>::Init()
{

    bool  initialized = true;
    //
    // будуємо шар ефекторів
    //

    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {

        m_Effectors.Add(0.0);
    }

    //
    // будуємо шар нейронів (стільки, скільки ефекторів)
    //

    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        CNeuron<Type>   *neuron;

        neuron = new CNeuron<Type>(0);

        if (neuron != NULL)
        {
            m_Neurons.Add(neuron);
        }
        else
        {
            initialized = false;
        }
    }

    if (initialized == false)
        return false;

    //
    // будуємо первинну матрицю вагів
    //
    m_weightMatrix.SetSize(m_InputsCount * m_InputsCount);

    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        for (SIZE_T j = 0; j < m_InputsCount; j++)
        {
            m_weightMatrix[i * m_InputsCount + j] = 0.0;
        }
    }


    m_Initialized = initialized;

    return initialized;
}


template <class Type>
void
CHopfieldNetwork<Type>::Teach(const CArray<Type>& data)
{

    //
    // виставляємо певні значення на ефекторах
    //
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        m_Effectors[i] = data[i];
    }



    //
    // обчислюємо значення на виході нейрону
    //
    for (SIZE_T j = 0; j < m_InputsCount; j++)    
    {

        Type  n = 0;

        for (SIZE_T i = j; i < m_InputsCount; i++)
        {
            n++;
            //
            // якщо головна діагональ
            //
            if (i == j)
            {
                m_weightMatrix[j * m_InputsCount + i] = 0;
            }
            else
            {
                Type sum = (m_Effectors[i] * m_Effectors[j]);

                m_weightMatrix[j * m_InputsCount + i] += (sum / n);
                m_weightMatrix[i * m_InputsCount + j] += (sum / n);
            }

        }

    }

}

template <class Type>
void
CHopfieldNetwork<Type>::Recovery(const CArray<Type>& input, CArray<Type>& output, CNetworkUpdateCallback<Type>* callbackClass)
{
    CArray<Type> outputTmp;

    m_Effectors.SetSize(input.GetSize());
    outputTmp.SetSize(input.GetSize());



    //
    // виставляємо певні значення на ефекторах
    //
    m_Effectors.Copy(input);
    outputTmp.Copy(input);


    //
    // відновлюємо масив
    //
    output.RemoveAll();
    output.SetSize(input.GetSize());


    //
    // виконуємо поки не буде стабільного стану
    //
    for ( ; ; )
    {
        //
        // обчислюємо значення на виході нейрону
        //
        for (SIZE_T j = 0; j < m_InputsCount; j++)
        {

            Type sum = 0;

            for (SIZE_T i = 0; i < m_InputsCount; i++)
            {

                if (i != j)
                {
                    sum += m_Effectors[i] * m_weightMatrix[j * m_InputsCount + i];
                }

            }


            outputTmp[j] = sign(sum);
            /*if (sum < 0)
            {
                outputTmp[j] = -1.0;
            }
            else
            {
                outputTmp[j] = 1.0;
            }*/

        }


        if (callbackClass != NULL)
        {
            callbackClass->NetworkUpdateCallback(outputTmp);
        }

        //
        // перевірка, що масиви ідентичні
        //
        bool identical = true;

        for (SIZE_T i = 0, j = m_Effectors.GetSize(); i < j; i++)
        {
            if (outputTmp[i] != m_Effectors[i])
            {
                identical = false;
                break;
            }
        }

        //
        // якщо не ідентичні, то копіюєюмо новий стан в ефектори і продовжуємо
        //
        if (!identical)
        {

            m_Effectors.Copy(outputTmp);
        }
        else
        {
            output.Copy(outputTmp);
            break;
        }

    }

}

#endif // __HOPFIELD_NETWORK_CPP__


#include "pch.h"
#ifndef __NEURONETS_IMPL_C__
#define __NEURONETS_IMPL_C__

#include "NeuronetsImpl.h"


template <class Type>
bool
CSolver<Type>::Initialize()
{
    //srand(GetTickCount());
    srand(1);

    m_TrainingDataInitialized = false;

    for (size_t i = 0; i < ARRAYSIZE(m_TrainingData); i++)
    {
        m_TrainingData[i].x1 = 0;
        m_TrainingData[i].x2 = 0;
        m_TrainingData[i].type = 0;
    }

    SetDefaults();

    return true;
}


template <class Type>
void
CSolver<Type>::InitializeTrainingData()
{


    /*for (size_t i = 0; i < ARRAYSIZE(m_TrainingData); i++)
    {
        Type x1 = ((Type)rand() / (RAND_MAX / 2.0)) - 1.0;
        Type x2 = ((Type)rand() / (RAND_MAX / 2.0)) - 1.0;

        bool Type = false;

        if ((x2 > x1 + 0.5) || (x2 < x1 - 0.5))
        {
            Type = true;
        }

        m_TrainingData[i].x1 = x1;
        m_TrainingData[i].x2 = x2;
        m_TrainingData[i].type = Type;
    }*/

    for (size_t i = 0; i < ARRAYSIZE(m_TrainingData); i++)
    {
        Type x1 = ((Type)rand() / (RAND_MAX / 1.0));
        Type x2 = ((Type)rand() / (RAND_MAX / 1.0));

        bool Type = true;   // Class C2

        if ((x2 > x1 + 0.5) || (x2 < x1 - 0.5))
        {
            Type = false;
        }

        m_TrainingData[i].x1 = x1;
        m_TrainingData[i].x2 = x2;
        m_TrainingData[i].type = Type;
    }

    m_TrainingDataInitialized = true;
    m_Trained = false;
}

template <class Type>
void
CSolver<Type>::SetDefaults()
{
    m_W10 = 0.1;
    m_W11 = -0.3;
    m_W12 = 0.4;

    m_W20 = -0.7;
    m_W21 = -0.1;
    m_W22 = 0.01;

    m_W10_2 = 0.4;
    m_W11_2 = -0.2;
    m_W12_2 = 0.1;

    m_Nu = 1.0;
    m_Epoc = 1000;

    m_Error.RemoveAll();
    m_TestError.RemoveAll();

}


template <class Type>
Type
CSolver<Type>::SigmaFunction(Type input)
{

    Type exponent = exp(-m_k * input);
    return 1.0 / (1.0 + exponent);
}

template <class Type>
Type
CSolver<Type>::DSigma(Type input)
{
    Type  sigma;

    sigma = SigmaFunction(input);

    return (sigma * m_k * (1.0 - sigma));
}


template <class Type>
void
CSolver<Type>::TrainNet()
{

    Type b1_1 = 1;
    Type b1_2 = 1;
    Type b2_1 = 1;

    m_Error.RemoveAll();
    m_TestError.RemoveAll();

    for (unsigned int epo = 0; epo < m_Epoc; epo++)
    {
        Type u1_1;
        Type u2_1;
        Type y1_1;
        Type y2_1;

        //
        // end result
        //
        Type y;

        Type u1_2;

        size_t  trainies = 0;
        Type error = 0.0;
        Type e;

        //
        // one epoc !
        //

        for (size_t i = 0; i < ARRAYSIZE(m_TrainingData); i++)
        {

            Type    d;

            Type x1 = m_TrainingData[i].x1;
            Type x2 = m_TrainingData[i].x2;

            d = 0.0;

            if (m_TrainingData[i].type == false)
            {
                d = 1.0;
            }

            //
            // forward pass
            //

            //
            // названіе переменно Ux_y
            // x - номер входа
            // y - номер слоя
            //

            //
            // layer 1
            //
            u1_1 = m_W10 * b1_1 + m_W11 * x1 + m_W12 * x2;
            u2_1 = m_W20 * b2_1 + m_W21 * x1 + m_W22 * x2;

            y1_1 = SigmaFunction(u1_1);
            y2_1 = SigmaFunction(u2_1);

            //
            // layer 2
            //
            u1_2 = m_W10_2 * b1_2 + m_W11_2 * y1_1 + m_W12_2 * y2_1;

            y = SigmaFunction(u1_2);
            e = d - y;

            error = error + e * e / 2;

            trainies++;


            //
            // backward pass
            // f'(u) = f(u) * k * [1 - f(u)]
            //

            //
            // layer 2
            //
            Type d_func2 = DSigma(u1_2);

            m_W10_2 = m_W10_2 + m_Nu * e * d_func2;
            m_W11_2 = m_W11_2 + m_Nu * e * d_func2 * y1_1;
            m_W12_2 = m_W12_2 + m_Nu * e * d_func2 * y2_1;

            //
            // layer 1
            //
            // u1
            Type d_func1 = DSigma(u1_1);

            m_W10 = m_W10 + m_Nu * e * d_func2 * m_W11_2 * d_func1;
            m_W11 = m_W11 + m_Nu * e * d_func2 * m_W11_2 * d_func1 * x1;
            m_W12 = m_W12 + m_Nu * e * d_func2 * m_W11_2 * d_func1 * x2;

            // u2
            d_func1 = DSigma(u2_1);

            m_W20 = m_W20 + m_Nu * e * d_func2 * m_W12_2 * d_func1;
            m_W21 = m_W21 + m_Nu * e * d_func2 * m_W12_2 * d_func1 * x1;
            m_W22 = m_W22 + m_Nu * e * d_func2 * m_W12_2 * d_func1 * x2;

        }

        //
        // check results
        //

        Type testError = 0.0; 

        // ------------------------------------
        //        x1 = 0    x2 = 0
        //
        // layer 1
        //
        u1_1 = m_W10 * b1_1 + m_W11 * 0 + m_W12 * 0;
        u2_1 = m_W20 * b2_1 + m_W21 * 0 + m_W22 * 0;

        y1_1 = SigmaFunction(u1_1);
        y2_1 = SigmaFunction(u2_1);

        //
        // layer 2
        //
        u1_2 = m_W10_2 * b1_2 + m_W11_2 * y1_1 + m_W12_2 * y2_1;
        y = SigmaFunction(u1_2);

        e = 0 - y;
        testError = e * e / 2;


        // ------------------------------------
        //        x1 = 1    x2 = 1
        //
        // layer 1
        //
        u1_1 = m_W10 * b1_1 + m_W11 * 1 + m_W12 * 1;
        u2_1 = m_W20 * b2_1 + m_W21 * 1 + m_W22 * 1;

        y1_1 = SigmaFunction(u1_1);
        y2_1 = SigmaFunction(u2_1);

        //
        // layer 2
        //
        u1_2 = m_W10_2 * b1_2 + m_W11_2 * y1_1 + m_W12_2 * y2_1;
        y = SigmaFunction(u1_2);

        e = 0 - y;
        testError = testError + e * e / 2;


        // ------------------------------------
        //        x1 = 0    x2 = 1
        //
        // layer 1
        //
        u1_1 = m_W10 * b1_1 + m_W11 * 0 + m_W12 * 1;
        u2_1 = m_W20 * b2_1 + m_W21 * 0 + m_W22 * 1;

        y1_1 = SigmaFunction(u1_1);
        y2_1 = SigmaFunction(u2_1);

        //
        // layer 2
        //
        u1_2 = m_W10_2 * b1_2 + m_W11_2 * y1_1 + m_W12_2 * y2_1;
        y = SigmaFunction(u1_2);

        e = 1 - y;
        testError = testError + e * e / 2;


        // ------------------------------------
        //        x1 = 1    x2 = 0
        //
        // layer 1
        //
        u1_1 = m_W10 * b1_1 + m_W11 * 1 + m_W12 * 0;
        u2_1 = m_W20 * b2_1 + m_W21 * 1 + m_W22 * 0;

        y1_1 = SigmaFunction(u1_1);
        y2_1 = SigmaFunction(u2_1);

        //
        // layer 2
        //
        u1_2 = m_W10_2 * b1_2 + m_W11_2 * y1_1 + m_W12_2 * y2_1;
        y = SigmaFunction(u1_2);

        e = 1 - y;
        testError = testError + e * e / 2;



        //
        // results
        //

        m_Error.Add(error / trainies);

        m_TestError.Add(testError / 4);

    }



    m_Trained = true;

}

template <class Type>
bool 
CSolver<Type>::CheckValue(Type x1, Type x2)
{

    Type b1_1 = 1;
    Type b2_1 = 1;
    Type b1_2 = 1;

    Type u1_1 = m_W10 * b1_1 + m_W11 * x1 + m_W12 * x2;
    Type u2_1 = m_W20 * b2_1 + m_W21 * x1 + m_W22 * x2;

    Type y1_1 = SigmaFunction(u1_1);
    Type y2_1 = SigmaFunction(u2_1);

    //
    // layer 2
    //
    Type u1_2 = m_W10_2 * b1_2 + m_W11_2 * y1_1 + m_W12_2 * y2_1;
    Type y = SigmaFunction(u1_2);

    return (y > 0.9) ? true : false;
}

#endif // __NEURONETS_IMPL_C__
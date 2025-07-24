#pragma once

#ifndef __NEURONETS_IMPL_H__
#define __NEURONETS_IMPL_H__

template <class Type>
class  CSolver
{

    typedef struct _TRAINING_DATA {
        Type       x1;
        Type       x2;

        bool       type;
    } TRAINING_DATA, * PTRAINING_DATA;

    bool        m_TrainingDataInitialized;
    bool        m_Trained;

    Type            m_Nu;
    Type            m_k;

    //
    // layer 1
    //
    Type            m_W10;
    Type            m_W11;
    Type            m_W12;
    Type            m_W20;
    Type            m_W21;
    Type            m_W22;

    //
    // layer 2
    //
    Type            m_W10_2;
    Type            m_W11_2;
    Type            m_W12_2;



    Type  SigmaFunction(Type input);
    Type  DSigma(Type input);

    unsigned int     m_Epoc;


public:

    //--------------------------------------------------------------
    // accessors
    //--------------------------------------------------------------
    void W10(Type data) { m_W10 = data; }
    Type W10() { return m_W10; }
    void W11(Type data) { m_W11 = data; }
    Type W11() { return m_W11; }
    void W12(Type data) { m_W12 = data; }
    Type W12() { return m_W12; }

    void W20(Type data) { m_W20 = data; }
    Type W20() { return m_W20; }
    void W21(Type data) { m_W21 = data; }
    Type W21() { return m_W21; }
    void W22(Type data) { m_W22 = data; }
    Type W22() { return m_W22; }

    void W10_2(Type data) { m_W10_2 = data; }
    Type W10_2() { return m_W10_2; }
    void W11_2(Type data) { m_W11_2 = data; }
    Type W11_2() { return m_W11_2; }
    void W12_2(Type data) { m_W12_2 = data; }
    Type W12_2() { return m_W12_2; }

    void Epoc(unsigned int data) { m_Epoc = data; }
    unsigned int  Epoc() { return m_Epoc; }

    void Nu(Type data) { m_Nu = data; }
    Type  Nu() { return m_Nu; }


    //
    // training data
    //
    TRAINING_DATA     m_TrainingData[1000];
    CArray<Type>      m_Error;
    CArray<Type>      m_TestError;

    CSolver<Type>()
    {
        m_TrainingDataInitialized = false;
        m_Trained = false;
        m_Epoc = 1000;
        m_Nu = 1.0;
        m_k = 1.0;
    }

    ~CSolver()
    {
        m_Error.RemoveAll();
        m_TestError.RemoveAll();
    }

    bool   IsTrainingData() {

        return m_TrainingDataInitialized;
    }

    bool   IsTrained() {

        return m_Trained;
    }

    void   InitializeTrainingData();

    void   TrainNet();
    bool   CheckValue(Type x1, Type x2);

    bool   Initialize();
    void   SetDefaults();



};

#include "NeuronetsImpl.cpp"

#endif // __NEURONETS_IMPL_H__

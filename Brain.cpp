#include "Brain.h"

void Synapse::initialize(Neuron *p,Neuron *t, float w)
{
    weight = w;
    next = NULL;
    pre = p;
    post = t;
}

void Synapse::handleSpikes()
{
    float p,str;
    if(pre->getSpike())
    {
        p = 1.0*rand()/RAND_MAX;
        str = -weight*logf(p);
        post->addInput(str);
    }
}

void Neuron::initialize(int nID, int pID, float xin, float yin, float zin)
{
    neuronID = nID;
    populationID = pID;
    x = xin;
    y = yin;
    z = zin;
    timeConstant = 10.0; //ms
    timeRefractory = 2.0; //ms
    threshold = -55.0; //mV
    resetPotential = -80.0; //mV
    restingPotential = -70.0; //mV
    spike = 0;
    potential = -80*rand()/RAND_MAX; //mV
    refractoryTimer = -1.0;
    input = 0.0; //mV
}

void Neuron::update(float dt)
{
    //reset spike flag and begin refractory period
    if(spike)
    {
        spike = 0;
        refractoryTimer = timeRefractory;
    }

    //update potential and generate spike
    if(refractoryTimer > 0)
    {
        potential = resetPotential;
        refractoryTimer -= dt;
    }
    else
    {
        potential += input;
        if(potential > threshold)
        {
            spike = 1;
            potential = resetPotential;
        }
        else
        {
            potential += -(potential - restingPotential)*(dt/timeConstant);
        }
    }

    //reset input buffer
    input = 0.0;
}

void NeuralPopulation::initialize(int num,int popID)
{
    numberOfNeurons = num;
    populationID = popID;
    neurons = new Neuron[numberOfNeurons];

    for(int n = 0; n < numberOfNeurons; ++n)
    {
        neurons[n].initialize(n,populationID,2.0*rand()/RAND_MAX -1.0,2.0*rand()/RAND_MAX-1.0,2.0*rand()/RAND_MAX-1.0);
    }
}

void NeuralPopulation::update(float dt)
{
    for(int n = 0; n < numberOfNeurons; ++n)
    {
        neurons[n].update(dt);
    }
}

void Brain::initialize()
{
    char buffer[256];
    int numNeurons;
    float dist,prob;
    float pos[3];
    Neuron *selectedNeuron1,*selectedNeuron2;
    Synapse *selectedSynapse;
    FILE *fin;

    updateCounter = 0;
    timeStep = 0.01; //ms
    extDrive = 1.5; //mV/ms
    random = new stdRan(0);

    fin = fopen("s10000.TOP","rb");

    fread(&numNeurons,sizeof(int),1,fin);
    fread(buffer,sizeof(int),2,fin);
    digitalInh.initialize(numNeurons,0);
    for(int n = 0; n < numNeurons; ++n)
    {
        fread(buffer,sizeof(int),1,fin);
        fread(pos,sizeof(float),3,fin);
        fread(buffer,sizeof(int),1,fin);
        fread(buffer,sizeof(int),10,fin);
        fread(buffer,sizeof(int),10,fin);
        fread(buffer,sizeof(int),10,fin);

        digitalInh.getNeuron(n)->setPosX(pos[0]);
        digitalInh.getNeuron(n)->setPosY(pos[1]);
        digitalInh.getNeuron(n)->setPosZ(pos[2]);
    }
    fclose(fin);

    synapseList = NULL;
    for(int n1 = 0; n1 < numNeurons; ++n1)
    {
        for(int n2 = 0; n2 < numNeurons; ++n2)
        {
            selectedSynapse = synapseList;
            selectedNeuron1 = digitalInh.getNeuron(n1);
            selectedNeuron2 = digitalInh.getNeuron(random->int32()%numNeurons);

            dist = (selectedNeuron1->getPosX() - selectedNeuron2->getPosX())*(selectedNeuron1->getPosX() - selectedNeuron2->getPosX());
            dist += (selectedNeuron1->getPosY() - selectedNeuron2->getPosY())*(selectedNeuron1->getPosY() - selectedNeuron2->getPosY());
            dist += (selectedNeuron1->getPosZ() - selectedNeuron2->getPosZ())*(selectedNeuron1->getPosZ() - selectedNeuron2->getPosZ());
            dist = sqrtf(dist);

            prob = random->doub();

            if(dist < 0.4)
            {
            synapseList = new Synapse;
            synapseList->initialize(selectedNeuron1,selectedNeuron2,-random->doub()*0.1);
            synapseList->setNext(selectedSynapse);
            }
        }
    }

    fclose(fin);
}

void Brain::update()
{
    Synapse *selectedSynapse;

    for(int i = 0; i < 1; ++i)
    {
        selectedSynapse = synapseList;
        while(selectedSynapse)
        {
            selectedSynapse->handleSpikes();
            selectedSynapse = selectedSynapse->getNext();
        }
        for(int n = 0; n < digitalInh.getNumberOfNeurons(); ++n)
        {
            digitalInh.getNeuron(n)->addInput(extDrive*timeStep);
        }
        digitalInh.update(timeStep);
    }
}

void Brain::resetRandom()
{
    for(int n = 0; n < digitalInh.getNumberOfNeurons(); ++n)
    {
        digitalInh.getNeuron(n)->setPotential(-30.0*random->doub()-60.0);
    }
}
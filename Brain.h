#ifndef _H_BRAIN
#define _H_BRAIN

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "StatRand.h"

class Synapse;
class Neuron;
class NeuralPopulation;
class NonLinearFilter;

class NonLinearFilter
{
public:
	void initialize(float mid, float sat, float slope)
	{
		midPoint = mid;
		satLevel = sat;
		steepness = slope;
	}

	float evaluate(float u)
	{
		float returnVal;

		returnVal = satLevel/(1 + expf(-4.0*steepness/satLevel*(u - midPoint)));

		return returnVal;
	}

	float evaluateDerivative(float u)
	{
		float returnVal;
		float exponentialVal,denomVal;

		exponentialVal = expf(-4.0*steepness/satLevel*(u - midPoint));
		denomVal = (1 + exponentialVal)*(1 + exponentialVal);
		returnVal = 4*steepness*exponentialVal/denomVal;

		return returnVal;
	}

	float getMidPoint(){return midPoint;}
    float getSatLevel(){return satLevel;}
    float getSteepness(){return steepness;}

private:
	float midPoint;
	float satLevel;
	float steepness;
};

class Synapse
{
private:
    float weight;
    Synapse *next;
    Neuron *pre,*post;
public:
    //BEHAVIOR
    void initialize(Neuron *p,Neuron *t, float w);
    void handleSpikes();
    //GET
    float getWeight(){return weight;}
    Synapse* getNext(){return next;}
    Neuron* getPost(){return pre;}
    Neuron* getPre(){return post;}
    //SET
    void setWeight(float w){weight = w;}
    void setNext(Synapse *n){next = n;}
    void setPost(Neuron *t){post = t;}
    void setPre(Neuron *p){pre = p;}
};

class Neuron
{
private:
    //CONSTANTS
    int neuronID;
    int populationID;
    float x,y,z;
    float timeConstant;
    float timeRefractory;
    float threshold;
    float resetPotential;
    float restingPotential;
    //VOLATILES
    int spike;
    float potential;
    float refractoryTimer;
    float input;
public:
    //BEHAVIOR
    void initialize(int nID, int pID, float xin, float yin, float zin);
    void addInput(float in){input += in;}
    void update(float dt);
    //GET
    int getSpike(){return spike;}
    float getPotential(){return potential;}
    float getPosX(){return x;}
    float getPosY(){return y;}
    float getPosZ(){return z;}
    //SET
    void setPotential(float p){potential = p;}
    void setPosX(float xin){x = xin;}
    void setPosY(float yin){y = yin;}
    void setPosZ(float zin){z = zin;}
};

class NeuralPopulation
{
private:
    //CONSTANTS
    int numberOfNeurons;
    int populationID;
    //ARRAYS
    Neuron* neurons;
public:
    //BEHAVIOR
    void initialize(int num,int popID);
    void update(float dt);
    //GET
    int getNumberOfNeurons(){return numberOfNeurons;}
    int getPopulationID(){return populationID;}
    Neuron* getNeuron(int n){return &neurons[n];}
};

class Brain
{
private:
    int updateCounter;
    float timeStep;
    float extDrive;
    stdRan* random;

    NeuralPopulation digitalInh;
    Synapse *synapseList;
public:
    void initialize();
    void update();
    void resetRandom();

    NeuralPopulation* getDigitalInh(){return &digitalInh;}

    //GET
    float getExtDrive(){return extDrive;}
    Synapse* getSynapseList(){return synapseList;}
    //SET
    void setExtDrive(float e){extDrive = e;}
};

#endif
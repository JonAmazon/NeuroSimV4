#include "PicoSimulation.h"

void PicoSimulation::initialize()
{

	//TEMPORARY FOR TESTING
    _mouseSensitivity = 1.0;
	_cam._position.setToZero();

	_cam._distance = 15.0;
	_cam._azimuth = 0.0;
	_cam._elevation = 25.0;

	_cam._FoV = 75.0;
	/////////////////////

    brain.initialize();

    updateCounter = 0;
}

void PicoSimulation::update(PicoInput* xinput,float dtin)
{
	_timeAccum += dtin;
	if(_timeAccum > 0.5){_timeAccum = 0.5;}

    brain.update();

    if(xinput->getKeyState(VK_RBUTTON))
    {
	    _cam._azimuth += _mouseSensitivity*(-xinput->getDeltaMposX());
	    _cam._elevation += _mouseSensitivity*(-xinput->getDeltaMposY());
	    _cam._distance += -xinput->getDeltaMwheel();
    }
    if(xinput->getKeyWasPressed('R'))
    {
        brain.resetRandom();
    }
    if(xinput->getKeyWasPressed(VK_UP))
    {
        brain.setExtDrive(brain.getExtDrive()+0.1);
        printf("ext drive: %f\n",brain.getExtDrive());
    }
    if(xinput->getKeyWasPressed(VK_DOWN))
    {
        brain.setExtDrive(brain.getExtDrive()-0.1);
        printf("ext drive: %f\n",brain.getExtDrive());
    }
    updateCounter++;
}

void PicoSimulation::release()
{
}
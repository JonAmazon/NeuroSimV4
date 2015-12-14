#ifndef _H_PICOSIMULATION
#define _H_PICOSIMULATION

#include "PicoGlobal.h"
#include "PicoInput.h"
#include "PicoVec.h"
#include "Brain.h"
#include <GL/gl.h>
#include <GL/glu.h>

struct PicoCamera
{
	float _FoV,_distance;
	float _azimuth,_elevation;
	PicoVec4f _position;
};

class PicoSimulation
{
public:
	void initialize();
	void update(PicoInput* xinput,float dt);
	void release();

    int getUpdateCounter(){return updateCounter;}
	PicoCamera* getxCamera(){return &_cam;}
    Brain* getBrain(){return &brain;}
private:
	int simPause;
    int updateCounter;
	float _timeAccum;
	float _mouseSensitivity;

	PicoCamera _cam;
    Brain brain;
};

#endif
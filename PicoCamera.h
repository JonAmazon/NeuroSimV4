#ifndef _H_PICOCAMERA
#define _H_PICOCAMERA

#include "PicoGlobal.h"

class PicoCamera
{
public:
	PICOerror initialize(float FoVin,float apectin);

private:
	float _FoV,_aspect;
	PicoVec3f _position;

};

#endif
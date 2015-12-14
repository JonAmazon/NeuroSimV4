#ifndef _H_PICORENDERER
#define _H_PICORENDERER

#include "PicoGlobal.h"
#include "PicoSimulation.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "AVIGenerator.h"


class PicoRenderer
{
public:
	void initialize(HWND hwndin, int widthin, int heightin);
	void update(PicoSimulation* xsimulation);
	void release();
private:
	int _width,_height;
	float _aspect;
	HDC _hDC;
	HGLRC _hGLRC;

    int AVIx,AVIy;
    int movieMake;
    CAVIGenerator AviGen;
	tagBITMAPINFOHEADER bih;
    BYTE* bmBits;

	void cameraView(PicoCamera* cam);
};

#endif
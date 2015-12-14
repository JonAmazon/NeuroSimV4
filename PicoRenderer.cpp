#include "PicoRenderer.h"

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void hueColor(float& R, float& G, float& B, double p, double sat)
{
	if(p <= 1.0/11.0){R = 1; G = 11.0/2.0*(p); B = 0;}
	else if(p <= 2.0/11.0){R = 1; G = 11.0/2.0*(p); B = 0;}
	else if(p <= 3.0/11.0){R = -11.0/2.0*(p-4.0/11.0); G = 1; B = 0;}
	else if(p <= 4.0/11.0){R = -11.0/2.0*(p-4.0/11.0); G = 1; B = 0;}
	else if(p <= 5.0/11.0){R = 0; G = 1; B = 11.0/2.0*(p-4.0/11.0);}
	else if(p <= 6.0/11.0){R = 0; G = 1; B = 11.0/2.0*(p-4.0/11.0);}
	else if(p <= 7.0/11.0){R = 0; G = -11.0/2.0*(p-8.0/11.0); B = 1;}
	else if(p <= 8.0/11.0){R = 0; G = -11.0/2.0*(p-8.0/11.0); B = 1;}
	else if(p <= 9.0/11.0){R = 11.0/2.0*(p-8.0/11.0); G = 0; B = 1;}
	else if(p <= 10.0/11.0){R = 11.0/2.0*(p-8.0/11.0); G = 0; B = 1;}
	else{R = 1; G = 0; B = 1;}
	R = sat*R+(1-sat); G = sat*G+(1-sat); B = sat*B+(1-sat);
}

void IndexColord(float& R, float& G, float& B, int idx, float sat)
{
	//find what RGB values correspond to idx
    R = 0.5; G = 0.5; B = 0.5;
    if((idx+1) == 1){R = 1.0; G = 0.0; B = 0.0;}
    if((idx+1) == 2){R = 1.0; G = 1.0; B = 0.0;}
    if((idx+1) == 3){R = 0.0; G = 1.0; B = 0.0;}
    if((idx+1) == 4){R = 0.0; G = 1.0; B = 1.0;}
    if((idx+1) == 5){R = 0.0; G = 0.0; B = 1.0;}
    if((idx+1) == 6){R = 1.0; G = 0.0; B = 1.0;}
    if((idx+1) == 7){R = 1.0; G = 1.0; B = 1.0;}
    
	//set saturation and RGB levels
	R = sat*R+(1-sat); G = sat*G+(1-sat); B = sat*B+(1-sat);
}

void drawSphere()
{
	int n = 16;
	float pi =  3.14159265358979323846264338327950288419716939937510;

	//draw body
	glBegin(GL_QUADS);
	for(int i = 0; i < 2*n; ++i)
	{
		for(int j = 1; j < n-1; ++j)
		{
			glNormal3f(sinf(j*pi/n)*sinf(i*pi/n),
					   sinf(j*pi/n)*cosf(i*pi/n),
					   cosf(j*pi/n));
			glVertex3f(sinf(j*pi/n)*sinf(i*pi/n),
					   sinf(j*pi/n)*cosf(i*pi/n),
					   cosf(j*pi/n));
			glVertex3f(sinf(j*pi/n)*sinf((i+1)*pi/n),
					   sinf(j*pi/n)*cosf((i+1)*pi/n),
					   cosf(j*pi/n));
			glVertex3f(sinf((j+1)*pi/n)*sinf((i+1)*pi/n),
					   sinf((j+1)*pi/n)*cosf((i+1)*pi/n),
					   cosf((j+1)*pi/n));
			glVertex3f(sinf((j+1)*pi/n)*sinf(i*pi/n),
					   sinf((j+1)*pi/n)*cosf(i*pi/n),
					   cosf((j+1)*pi/n));
		}
	}
	glEnd();

	//draw caps
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0,1.0,1.0);
	for(int i = 0; i < 2*n; ++i)
	{
		glNormal3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,1.0);
		glVertex3f(sinf((1)*pi/n)*sinf((i+1)*pi/n),
					   sinf((1)*pi/n)*cosf((i+1)*pi/n),
					   cosf((1)*pi/n));
		glVertex3f(sinf((1)*pi/n)*sinf(i*pi/n),
				   sinf((1)*pi/n)*cosf(i*pi/n),
				   cosf((1)*pi/n));
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0,1.0,1.0);
	for(int i = 0; i < 2*n; ++i)
	{
		glNormal3f(0.0,0.0,-1.0);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(-sinf((1)*pi/n)*sinf((i+1)*pi/n),
				   -sinf((1)*pi/n)*cosf((i+1)*pi/n),
				   -cosf((1)*pi/n));
		glVertex3f(-sinf((1)*pi/n)*sinf(i*pi/n),
				   -sinf((1)*pi/n)*cosf(i*pi/n),
				   -cosf((1)*pi/n));
	}
	glEnd();
}

void drawBox()
{
	glBegin(GL_QUADS);
	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(-1.0,-1.0,-1.0);
	glVertex3f(1.0,-1.0,-1.0);
	glVertex3f(1.0,-1.0,1.0);
	glVertex3f(-1.0,-1.0,1.0);

	glNormal3f(0.0,1.0,0.0);
	glVertex3f(-1.0,1.0,-1.0);
	glVertex3f(1.0,1.0,-1.0);
	glVertex3f(1.0,1.0,1.0);
	glVertex3f(-1.0,1.0,1.0);
	glEnd();
}

void drawLineCube()
{
	
	glBegin(GL_LINES);
	glColor3f(1.0,1.0,1.0);

	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);

	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,0.0);

	glVertex3f(1.0,1.0,0.0);
	glVertex3f(0.0,1.0,0.0);

	glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);

	glVertex3f(0.0,0.0,1.0);
	glVertex3f(1.0,0.0,1.0);

	glVertex3f(1.0,0.0,1.0);
	glVertex3f(1.0,1.0,1.0);

	glVertex3f(1.0,1.0,1.0);
	glVertex3f(0.0,1.0,1.0);

	glVertex3f(0.0,1.0,1.0);
	glVertex3f(0.0,0.0,1.0);

	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);

	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,0.0,1.0);

	glVertex3f(1.0,1.0,0.0);
	glVertex3f(1.0,1.0,1.0);

	glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,1.0,1.0);

	glColor3f(1.0,1.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,1.0,1.0);

	glEnd();
}

void drawLineAxes()
{
    glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3f(1.0,0.7,0.7);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);

    glColor3f(0.7,1.0,0.7);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);

    glColor3f(0.7,0.7,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);

	glEnd();
}

void drawNeuralPopulation(NeuralPopulation *pop)
{
    int numN = pop->getNumberOfNeurons();
    int spike;
    float R,G,B;
    Neuron *sel;

    glPointSize(5);
    glBegin(GL_POINTS);
    for(int n = 0; n < numN; ++n)
    {
        sel = pop->getNeuron(n);
        spike = sel->getSpike();
        if(spike){R = 1.0; G = 0.0; B = 0.0;}
        else
        {
            R = 0.2;
            B = 0.8*(sel->getPotential()+80.0)/(-55.0+80.0)+0.2;
            G = 0.2;
        }
        glColor3f(R,G,B);
        glVertex3f(sel->getPosX(),sel->getPosY(),sel->getPosZ());
    }
    glEnd();
}

void drawSynapses(Neuron* npre, Synapse* syn)
{
    float R,G,B;
    Synapse *selSyn = syn;
    Neuron *npost;

    glLineWidth(1.0);
    glBegin(GL_LINES);
    while(selSyn)
    {
        glColor3f(1.0,1.0,1.0);
        if(selSyn->getPre() == npre)
        {
            npost = selSyn->getPost();
            glVertex3f(npre->getPosX(),npre->getPosY(),npre->getPosZ());
            glVertex3f(npost->getPosX(),npost->getPosY(),npost->getPosZ());
        }
        selSyn = selSyn->getNext();
    }
    glEnd();
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicoRenderer::initialize(HWND hwndin,int widthin,int heightin)
{
	int pixelFormat;
	
	_width = widthin;
	_height = heightin;
	_aspect = float(_width)/_height;
	_hDC = GetDC(hwndin);

	PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  /* size */
        1,                              /* version */
        PFD_SUPPORT_OPENGL |
        PFD_DRAW_TO_WINDOW |
        PFD_DOUBLEBUFFER,               /* support double-buffering */
        PFD_TYPE_RGBA,                  /* color type */
        16,                             /* prefered color depth */
        0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
        0,                              /* no alpha buffer */
        0,                              /* alpha bits (ignored) */
        0,                              /* no accumulation buffer */
        0, 0, 0, 0,                     /* accum bits (ignored) */
        16,                             /* depth buffer */
        0,                              /* no stencil buffer */
        0,                              /* no auxiliary buffers */
        PFD_MAIN_PLANE,                 /* main layer */
        0,                              /* reserved */
        0, 0, 0,                        /* no layer, visible, damage masks */
    };
    pixelFormat = ChoosePixelFormat(_hDC, &pfd);
	SetPixelFormat(_hDC, pixelFormat, &pfd);

	_hGLRC = wglCreateContext(_hDC);
	wglMakeCurrent(_hDC, _hGLRC);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
    glShadeModel(GL_SMOOTH);
    GLfloat lightPos[] = {0.0,1.0,0.0,0.0};
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glViewport(0, 0, _width, _height);

    AVIx = _width;
    AVIy = _height;
    TCHAR nameavi[] = TEXT("movie.avi");
    AviGen.SetRate(60); //fps
	AviGen.SetFileName(nameavi);
    memset(&bih,NULL,sizeof(tagBITMAPINFOHEADER));
	bih.biBitCount = 24;
	bih.biHeight = AVIy;
	bih.biWidth = AVIx;
	bih.biPlanes = 1;
	bih.biCompression = BI_RGB;
	bih.biSize = sizeof(tagBITMAPINFOHEADER);
	bih.biSizeImage = AVIx*AVIy*3;
	bih.biXPelsPerMeter = 2835;
	bih.biYPelsPerMeter = 2835;
	AviGen.SetBitmapHeader(&bih);// get bitmap info out of the view
	//AviGen.InitEngine();	// start engine
    bmBits = new BYTE[AVIx*AVIy*3];
    memset(bmBits,0,AVIx*AVIy*3);
    movieMake = 0;
}

void PicoRenderer::update(PicoSimulation* xsimulation)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	glPushMatrix();
	cameraView(xsimulation->getxCamera());	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
    drawLineAxes();
    drawNeuralPopulation(xsimulation->getBrain()->getDigitalInh());
    //drawSynapses(xsimulation->getBrain()->getDigitalInh()->getNeuron(0),xsimulation->getBrain()->getSynapseList());
	glEnable(GL_LIGHTING);

	glPopMatrix();
	glFlush();
	SwapBuffers(_hDC);
    
    if(movieMake)
    {
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
	    glReadPixels(0,0,AVIx,AVIy,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmBits);
        AviGen.AddFrame(bmBits);
    }
}

void PicoRenderer::cameraView(PicoCamera* xcam)
{
	float angle;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(xcam->_FoV, _aspect, 0.1, 1000.0);

	glTranslatef(0.0,0.0,-xcam->_distance);
	glRotatef(xcam->_elevation,1.0,0.0,0.0);
	glRotatef(-xcam->_azimuth,0.0,1.0,0.0);
	glTranslatef(-xcam->_position.x,-xcam->_position.y,-xcam->_position.z);
}

void PicoRenderer::release()
{
}


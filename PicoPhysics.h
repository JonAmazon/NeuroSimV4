#ifndef _H_PICOPHYSICS
#define _H_PICOPHYSICS

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <windows.h>
#include "PicoVec.h"
#include "PicoMat.h"

struct PolygonIndices
{
	int vertexIndices[3];
	int normalIndices[3];
};

class Mesh
{
public:
	void initialize();
	void release();

	int getNumVertices(){return numVertices;}
	int getNumNormals(){return numNormals;}
	int getNumFaces(){return numFaces;}

	PicoVec4f* getVertexArray(){return vertices;}
	PicoVec4f* getNormalArray(){return normals;}
	PolygonIndices* getFaceArray(){return faces;}
private:
	int numVertices;
	int numNormals;
	int numFaces;

	PicoVec4f* vertices;
	PicoVec4f* normals;
	PolygonIndices* faces;
};

class PicoPhysicsObject
{
public:
	int isStatic;
	int isColliding;

	float mass;
	//TODO: this should technically be a 4x4 matrix!
	float inertia;

	PicoVec4f position;
	PicoVec4f momentum;
	PicoVec4f velocity;
	PicoVec4f force;
	PicoVec4f orientation;
	PicoVec4f spin;
	PicoVec4f angularVelocity;
	PicoVec4f angularMomentum;
	PicoVec4f torque;
	PicoVec4f scale;

	PicoMat4f modelToWorldMatrix;
	Mesh collisionMesh;

	void applyForceAtWorldPoint(PicoVec4f worldPoint,PicoVec4f forceIn)
	{
		force += forceIn;
		torque += (worldPoint - position).crossProduct(forceIn);
	}

	void applyForceAtCenterOfMass(PicoVec4f forceIn)
	{
		force += forceIn;
	}

	void updateModelToWorldMatrix()
	{
		modelToWorldMatrix.setToTransformation(scale,orientation,position);
	}

	PicoVec4f getVelocityAtWorldPoint(PicoVec4f worldpoint)
	{
		return velocity+angularVelocity.crossProduct(worldpoint-position);
	}
};

class collisionStatus
{
public:
	void initialize();
	void trackObjects(PicoPhysicsObject *Ain, PicoPhysicsObject *Bin);
	void findClosestWorldPoint();
	void resolveCollision(float timeStep);

	int isColliding(){return colliding;}
	PicoPhysicsObject* getObjectA(){return A;}
	PicoPhysicsObject* getObjectB(){return B;}
	PicoVec4f getClosestPointA(){return closestPointA;}
	PicoVec4f getClosestPointB(){return closestPointB;}
	PicoVec4f getCollisionNormal(){return collisionNormal;}
private:
	PicoPhysicsObject *A,*B;
	int colliding;
	float *weightsA,*weightsB;
	PicoVec4f closestPointA;
	PicoVec4f closestPointB;
	PicoVec4f collisionNormal;
};

class PicoPhysics
{
public:
	void initialize();
	void runIntegrationStep(PicoPhysicsObject* objectsin, int nobjects);
	void release();

	float getTimeStep(){return timeStep;}

	collisionStatus* getCollisionStatus(){return &collision;}
private:
	float timeStep;
	collisionStatus collision;

	void resolveCollision();

};

#endif
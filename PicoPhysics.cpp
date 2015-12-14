#include "PicoPhysics.h"

void PicoPhysics::initialize()
{
	timeStep = 0.005;
	collision.initialize();
}

void PicoPhysics::runIntegrationStep(PicoPhysicsObject* objectsin, int nobjects)
{
	for(int i = 0; i < nobjects; ++i)
	{
		if(objectsin[i].isStatic){}
		else{
		objectsin[i].momentum += (objectsin[i].force)*(0.5*timeStep);
		objectsin[i].velocity = (objectsin[i].momentum)/(objectsin[i].mass);
		objectsin[i].position += (objectsin[i].momentum)*(timeStep);
		objectsin[i].momentum += (objectsin[i].force)*(0.5*timeStep);
		objectsin[i].force.setToZero();

		objectsin[i].angularMomentum += (objectsin[i].torque)*(0.5*timeStep);
		objectsin[i].angularVelocity = objectsin[i].angularMomentum/(objectsin[i].inertia);
		objectsin[i].spin = (objectsin[i].angularVelocity).quaternionProduct(objectsin[i].orientation);
		objectsin[i].orientation += (objectsin[i].spin)*(0.5*timeStep);
		objectsin[i].orientation.normalize();
		objectsin[i].angularMomentum += (objectsin[i].torque)*(0.5*timeStep);
		objectsin[i].torque.setToZero();
		}

		objectsin[i].updateModelToWorldMatrix();
	}
	
	for(int i = 0; i < nobjects; ++i)
	{
		for(int j = i+1; j < nobjects; ++j)
		{
			collision.trackObjects(&objectsin[i],&objectsin[j]);
		}
	}
	collision.findClosestWorldPoint();
	//if(collision.isColliding())
	//{collision.resolveCollision(timeStep);}
}

void collisionStatus::resolveCollision(float timeStep)
{
	float restitution = 0.2, reducedMass, impulse;
	float distance, velocityNormal, depth;
	PicoVec4f relativeVelocity;

	relativeVelocity = (A->getVelocityAtWorldPoint(closestPointA)) - (B->getVelocityAtWorldPoint(closestPointA));

	if((B->isStatic)||(A->isStatic)){reducedMass = 1.0;}
	else{reducedMass = (A->mass)*(B->mass)/(A->mass + B->mass);}
	velocityNormal = (collisionNormal.innerProduct(relativeVelocity));
	impulse = -(1+restitution)*reducedMass*velocityNormal;

	if(!(A->isStatic))
	{
		A->applyForceAtWorldPoint(closestPointA,collisionNormal*impulse/timeStep);
	}
	if(!(B->isStatic))
	{
		B->applyForceAtWorldPoint(closestPointB,-collisionNormal*impulse/timeStep);
	}

	printf("velocityNormal = %f\n",impulse);
}

void collisionStatus::findClosestWorldPoint()
{
	colliding = 0;
	int AbehindBplane = 0;
	int BbehindAplane = 0;
	int iteration = 0;
	int isDoneIterating = 0;
	int nA = A->collisionMesh.getNumVertices();
    int nB = B->collisionMesh.getNumVertices();

    float wAsum;
    float wBsum;
	float multiplier,Amultiplier,Bmultiplier;

	PicoVec4f* AvertModel = A->collisionMesh.getVertexArray();
    PicoVec4f* BvertModel = B->collisionMesh.getVertexArray();
	PicoVec4f* AvertWorld;
	PicoVec4f* BvertWorld;

    PicoVec4f guessA;
    PicoVec4f guessB;
	PicoVec4f displacementPrev;
    PicoVec4f displacement;

	AvertWorld = new PicoVec4f[nA];
	BvertWorld = new PicoVec4f[nB];

	for(int i = 0; i < nA; ++i)
	{
		AvertModel[i].w = 1.0;
		AvertWorld[i] = A->modelToWorldMatrix.matrixVectorProduct(AvertModel[i]);
	}

	for(int i = 0; i < nB; ++i)
	{
		BvertModel[i].w = 1.0;
		BvertWorld[i] = B->modelToWorldMatrix.matrixVectorProduct(BvertModel[i]);
	}

	
    guessA.setToZero();
    guessB.setToZero();
	wAsum = 0;
	wBsum = 0;

    for(int i = 0; i < nA; ++i)
    {
		guessA += AvertWorld[i]*weightsA[i];
		wAsum += weightsA[i];
    }

	for(int i = 0; i < nB; ++i)
    {
		guessB += BvertWorld[i]*weightsB[i];
		wBsum += weightsB[i];
    }

	guessA /= wAsum;
	guessB /= wBsum;

	displacementPrev.setToZero();
    displacement = guessA-guessB;

	while(!isDoneIterating)
    {
		multiplier = 0.01/(iteration+1);
		if(BbehindAplane){Amultiplier = multiplier;}
		else{Amultiplier = multiplier;}
		
		if(AbehindBplane){Bmultiplier = multiplier;}
		else{Bmultiplier = multiplier;}

        wAsum = 0.0;
		AbehindBplane = 0;
        for(int i = 0; i < nA; ++i)
        {
            if((guessA-AvertWorld[i]).innerProduct(displacement) < 0.0)
            {weightsA[i] -= Amultiplier;}
			else
            {weightsA[i] += Amultiplier;}

			if((guessB-AvertWorld[i]).innerProduct(displacement) > 0.0)
			{guessA = AvertWorld[i]; AbehindBplane = 1; colliding = 1;}

			if(weightsA[i] < 0.0){weightsA[i] = 0.0;}
            wAsum += weightsA[i];
        }

        wBsum = 0.0;
		BbehindAplane = 0;
        for(int i = 0; i < nB; ++i)
        {
            if((guessB-BvertWorld[i]).innerProduct(-displacement) < 0.0)
            {weightsB[i] -= Bmultiplier;}
			else
            {weightsB[i] += Bmultiplier;}

			if((guessA-BvertWorld[i]).innerProduct(-displacement) > 0.0)
			{guessB = BvertWorld[i]; BbehindAplane = 1; colliding = 1;}

			if(weightsB[i] < 0.0){weightsB[i] = 0.0;}
            wBsum += weightsB[i];
        }

		if(AbehindBplane){}
		else
		{
			guessA.setToZero();
			for(int i = 0; i < nA; ++i)
			{
				guessA += AvertWorld[i]*weightsA[i];
				weightsA[i] /= wAsum;
			}
		}

		if(BbehindAplane){}
		else
		{
			guessB.setToZero();
			for(int i = 0; i < nB; ++i)
			{
				guessB += BvertWorld[i]*weightsB[i];
				weightsB[i] /= wBsum;
			}
		}

        guessA /= wAsum;
        guessB /= wBsum;
		
		displacementPrev = displacement;
        displacement = guessA-guessB;
        iteration++;

		if((displacementPrev-displacement).lengthSqrd() < (0.005*0.005)){isDoneIterating = 1;}
		if(iteration > 1000){isDoneIterating = 1;}
    }
	
	collisionNormal = displacement;
	collisionNormal.normalize();
	closestPointA = guessA;
	closestPointB = guessB;

	printf("iterations = %d ... %f\n\n",iteration,displacement.length());

	delete [] AvertWorld;
	delete [] BvertWorld;
}

void PicoPhysics::release()
{}

void collisionStatus::initialize()
{
	A = NULL;
	B = NULL;
	weightsA = NULL;
	weightsB = NULL;
}

void collisionStatus::trackObjects(PicoPhysicsObject *Ain, PicoPhysicsObject *Bin)
{
	if((A == Ain)&&(B == Bin)){}
	else{
		A = Ain;
		B = Bin;
		int nA = A->collisionMesh.getNumVertices();
		int nB = B->collisionMesh.getNumVertices();
		colliding = 0;

		if(weightsA){delete [] weightsA;}
		if(weightsB){delete [] weightsB;}

		weightsA = new float[nA];
		weightsB = new float[nB];

		for(int i = 0; i < nA; ++i)
		{
			weightsA[i] = 1.0;
		}
		for(int i = 0; i < nB; ++i)
		{
			weightsB[i] = 1.0;
		}

		closestPointA.setToZero();
		closestPointB.setToZero();
		collisionNormal.setToZero();
	}
}

void Mesh::initialize()
{
	numVertices = 8;
	numNormals = 6;
	numFaces = 12;
	
	vertices = new PicoVec4f[numVertices];
	normals = new PicoVec4f[numNormals];
	faces = new PolygonIndices[numFaces];

	vertices[0] = PicoVec4f(-1.0,-1.0,-1.0,0.0);
	vertices[1] = PicoVec4f(1.0,-1.0,-1.0,0.0);
	vertices[2] = PicoVec4f(1.0,-1.0,1.0,0.0);
	vertices[3] = PicoVec4f(-1.0,-1.0,1.0,0.0);
	vertices[4] = PicoVec4f(-1.0,1.0,-1.0,0.0);
	vertices[5] = PicoVec4f(1.0,1.0,-1.0,0.0);
	vertices[6] = PicoVec4f(1.0,1.0,1.0,0.0);
	vertices[7] = PicoVec4f(-1.0,1.0,1.0,0.0);

	normals[0] = PicoVec4f(0.0,-1.0,0.0,0.0);
	normals[1] = PicoVec4f(0.0,1.0,0.0,0.0);
	normals[2] = PicoVec4f(-1.0,0.0,0.0,0.0);
	normals[3] = PicoVec4f(1.0,0.0,0.0,0.0);
	normals[4] = PicoVec4f(0.0,0.0,-1.0,0.0);
	normals[5] = PicoVec4f(0.0,0.0,1.0,0.0);

	faces[0].vertexIndices[0] = 0;
	faces[0].normalIndices[0] = 0;
	faces[0].vertexIndices[1] = 2;
	faces[0].normalIndices[1] = 0;
	faces[0].vertexIndices[2] = 3;
	faces[0].normalIndices[2] = 0;

	faces[1].vertexIndices[0] = 0;
	faces[1].normalIndices[0] = 0;
	faces[1].vertexIndices[1] = 1;
	faces[1].normalIndices[1] = 0;
	faces[1].vertexIndices[2] = 2;
	faces[1].normalIndices[2] = 0;

	faces[2].vertexIndices[0] = 4;
	faces[2].normalIndices[0] = 1;
	faces[2].vertexIndices[1] = 7;
	faces[2].normalIndices[1] = 1;
	faces[2].vertexIndices[2] = 6;
	faces[2].normalIndices[2] = 1;

	faces[3].vertexIndices[0] = 4;
	faces[3].normalIndices[0] = 1;
	faces[3].vertexIndices[1] = 6;
	faces[3].normalIndices[1] = 1;
	faces[3].vertexIndices[2] = 5;
	faces[3].normalIndices[2] = 1;

	faces[4].vertexIndices[0] = 0;
	faces[4].normalIndices[0] = 2;
	faces[4].vertexIndices[1] = 3;
	faces[4].normalIndices[1] = 2;
	faces[4].vertexIndices[2] = 7;
	faces[4].normalIndices[2] = 2;

	faces[5].vertexIndices[0] = 0;
	faces[5].normalIndices[0] = 2;
	faces[5].vertexIndices[1] = 7;
	faces[5].normalIndices[1] = 2;
	faces[5].vertexIndices[2] = 4;
	faces[5].normalIndices[2] = 2;

	faces[6].vertexIndices[0] = 1;
	faces[6].normalIndices[0] = 3;
	faces[6].vertexIndices[1] = 6;
	faces[6].normalIndices[1] = 3;
	faces[6].vertexIndices[2] = 2;
	faces[6].normalIndices[2] = 3;

	faces[7].vertexIndices[0] = 1;
	faces[7].normalIndices[0] = 3;
	faces[7].vertexIndices[1] = 5;
	faces[7].normalIndices[1] = 3;
	faces[7].vertexIndices[2] = 6;
	faces[7].normalIndices[2] = 3;

	faces[8].vertexIndices[0] = 0;
	faces[8].normalIndices[0] = 4;
	faces[8].vertexIndices[1] = 4;
	faces[8].normalIndices[1] = 4;
	faces[8].vertexIndices[2] = 5;
	faces[8].normalIndices[2] = 4;

	faces[9].vertexIndices[0] = 0;
	faces[9].normalIndices[0] = 4;
	faces[9].vertexIndices[1] = 5;
	faces[9].normalIndices[1] = 4;
	faces[9].vertexIndices[2] = 1;
	faces[9].normalIndices[2] = 4;

	faces[10].vertexIndices[0] = 3;
	faces[10].normalIndices[0] = 5;
	faces[10].vertexIndices[1] = 6;
	faces[10].normalIndices[1] = 5;
	faces[10].vertexIndices[2] = 7;
	faces[10].normalIndices[2] = 5;

	faces[11].vertexIndices[0] = 3;
	faces[11].normalIndices[0] = 5;
	faces[11].vertexIndices[1] = 2;
	faces[11].normalIndices[1] = 5;
	faces[11].vertexIndices[2] = 6;
	faces[11].normalIndices[2] = 5;
}

void Mesh::release()
{
	delete [] vertices;
	delete [] normals;
	delete [] faces;
}
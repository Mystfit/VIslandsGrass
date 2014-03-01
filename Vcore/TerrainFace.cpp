//
//  TerrainFace.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TerrainFace.h"
#include <iostream>

using namespace Vcore;

TerrainFace::TerrainFace(Vertex* a, Vertex* b, Vertex* c): Face(a, b, c)
{
}

Vec3 TerrainFace::getRandomPoint(Face* face, float distX, float distY)
{
	//Get a random percent between 0 and 1
	float a = distX;
	float b = distY;

	Vec3 result;
	Vec3* resultPointer;

	//If the sum total of both points are over one then invert. 
	//This is to get the correct barycentric coordinates in the face
	if( (a+b) >= 1){
		a = 1 - a;
		b = 1 - b;
	}

	//Get subtracted vectors from origin
	Vec3 u = *(face->getVertex(2)) - *(face->getVertex(0));
	Vec3 v = *(face->getVertex(1)) - *(face->getVertex(0));

	//Scale length of edge vectors by the random amount
	u = u.normalize( u.length() * a );
	v = v.normalize( v.length() * b );

	//Add the scaled vectors back to the origin to get the random point in world space
	result = ( *(face->getVertex(0)) + u ) + v;

	return result;
}

float TerrainFace::randf(){
    return ((float)rand()/(float)RAND_MAX);
}

//Averages height of all corner vertices to get average height of face
float TerrainFace::getAveragedHeight(Face* face)       
{
	return (face->getVertex(0)->y + face->getVertex(1)->y + face->getVertex(2)->y ) / 3;
}


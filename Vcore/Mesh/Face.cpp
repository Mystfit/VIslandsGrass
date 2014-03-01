//
//  Mesh.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Face.h"
#include <iostream>

using namespace Vcore;

Face::Face(Vertex* a, Vertex* b, Vertex* c)
{
    m_faceVertices[0] = a;
    m_faceVertices[1] = b;
    m_faceVertices[2] = c;


    a->setColour(0.5f,0.5f,0.5f);
    b->setColour(0.5f,0.5f,0.5f);
    c->setColour(0.5f,0.5f,0.5f);
}


void Face::calcFaceNormal()
{
	//m_faceNormal = Vec3::cross(  *(m_faceVertices[1]) - *(m_faceVertices[0]), *(m_faceVertices[2]) - *(m_faceVertices[0]) ).normalize();

    m_faceNormal = Vec3::normal(*m_faceVertices[1], *m_faceVertices[2], *m_faceVertices[0]);
	//Add the face normal to each vertice making this face. Each new normal added is averaged with the existing one.
	m_faceVertices[0]->addFaceNormal(m_faceNormal);
	m_faceVertices[1]->addFaceNormal(m_faceNormal);
	m_faceVertices[2]->addFaceNormal(m_faceNormal);
}


Vec3 Face::pointOnFace(Face* face, float distX, float distY)
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


float Face::getAveragedHeight(Face* face)
{
    return (face->getVertex(0)->y + face->getVertex(1)->y + face->getVertex(2)->y ) / 3;
}

Vec3 Face::getFaceMidPoint( Face* face)
{
    float midX = (face->getVertex(0)->x + face->getVertex(1)->x + face->getVertex(2)->x) / 3;
    float midY = (face->getVertex(0)->y + face->getVertex(1)->y + face->getVertex(2)->y) / 3;
    float midZ = (face->getVertex(0)->z + face->getVertex(1)->z + face->getVertex(2)->z) / 3;
    return Vec3(midX, midY, midZ);
}

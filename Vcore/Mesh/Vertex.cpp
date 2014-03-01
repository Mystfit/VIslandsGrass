//
//  Vertex.cpp
//  Vcore
//
//  Created by Byron Mallett on 21/03/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Vertex.h"
#include <iostream>

using namespace Vcore;

Vertex::Vertex()
{
    x = y = z = 0.0f;
    setColour(1,1,1,0.2f);
}

Vertex::Vertex(float x_pos, float y_pos, float z_pos): Vec3(x_pos, y_pos, z_pos)
{
}

void Vertex::addFaceNormal(Vec3 faceNormal)
{
	m_normal = (m_normal + faceNormal).normalize();
}


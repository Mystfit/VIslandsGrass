//
//  TerrainFace.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GrassStalk.h"

using namespace Vcore;

GrassSegment::GrassSegment()
{
}

GrassStalk::GrassStalk(
	Vec3 position, 
	int numSegments, 
	float segmentHeight, 
	float baseWidth, 
	float bendAmount, 
	Vec3 growthDirection, 
	int clock, 
	Vec3 windVector, 
	int curveInterp, 
	float colourScale,
	Vec3 grassBaseColour1,
    Vec3 grassBaseColour2,
    Vec3 grassTipColour1,
    Vec3 grassTipColour2
): Mesh(position)
{

    //m_min_startColour = Vec3(0.0f,0.251f,0.167f);
    //m_max_startColour = Vec3(0.275f,0.243f,0.043f);
    //m_min_endColour = Vec3(0.395f,0.551f,0.257f);
    //m_max_endColour = Vec3(0.611f,0.587,0.257f);

    m_min_startColour = grassBaseColour1;
    m_max_startColour = grassBaseColour2;
    m_min_endColour = grassTipColour1;
    m_max_endColour = grassTipColour2;

    //std::cout << "Colour r:" << grassBaseColour1.x << " g:" << grassBaseColour1.y << " z:" << grassBaseColour1.z << std::endl;


	//Clear vectors
	m_numSegments = numSegments;
	m_curveType = curveInterp;
	m_segments.clear();
	m_segments.resize(numSegments);
	m_vertices.clear();
	m_vertices.resize(numSegments);

	Vertex * newVertex;
	int i;
	int numWidths;
	float h;
	float currentAngle;
	float segmentLength = segmentHeight;
	//baseWidth = 0.6f;
	//std::cout << "Width:" << baseWidth << std::endl;

	//float baseWidth = 0.6f;
	float maxBend = Vec3::degToRad(15.0f);

	//std::cout << "Wind Vector x:" << windVector.x << " y:" << windVector.y << " z:" << windVector.z << std::endl;

	//float waveOffset = sin((float)clock/25.0f)*0.5+0.5; 
	//float bendFactor = Vec3::degToRad((maxBend + windVector.length()) * waveOffset) ;
	
	//float bendFactor = Vec3::degToRad( windVector.length() / maxBend ) ;
	float bendFactor = windVector.length() * maxBend;

	//float bendDirection = windVector.horizAngle();l
	float bendDirection = growthDirection.horizAngle()-M_PI;
	//bendDirection = 0.0f;

	//Vec3 normalTangent(cos(bendDirection - M_PI), 0, sin(bendDirection - M_PI/2));

	//Vec3::normal(m_segments[i].direction, Vec3(cos(m_segments[i].direction.horizAngle()), 0.0f, sin(m_segments[i].direction.horizAngle())));


	numWidths = getNumCubicSegments();

	for (i = 0; i < numSegments; i++ )
	{
		if(i > 0){
			m_segments[i].position = m_segments[i-1].position + m_segments[i-1].direction;
			currentAngle = m_segments[i-1].direction.vertAngle();
		} else {
			m_segments[i].position = position;
			m_segments[i].direction = growthDirection.normalize(segmentLength);
			currentAngle = m_segments[i].direction.vertAngle();
		}

		//Bend the current segment by the angle increment
		currentAngle += bendFactor + Vec3::degToRad(3.0f);
		m_segments[i].direction.y = sin (currentAngle) * segmentLength;

		//Set the horizontal rotation of the grass
		h = cos (currentAngle) * segmentLength;
		//m_segments[i].direction.x = cos( Vec3::lerp( (i+1.0f)/numSegments, bendDirection, windVector.horizAngle()-M_PI) ) * h;
		//m_segments[i].direction.z = sin( Vec3::lerp( (i+1.0f)/numSegments, bendDirection, windVector.horizAngle()-M_PI) ) * h;
		m_segments[i].direction.x = cos( windVector.horizAngle() ) * h;
		m_segments[i].direction.z = sin( windVector.horizAngle() ) * h;

		//Create the grass vertex
		newVertex = new Vertex(m_segments[i].position.x, m_segments[i].position.y, m_segments[i].position.z);
		
		//Need copy of normal set for maya
		newVertex->setNormal( Vec3::normal(m_segments[i].direction, 
			Vec3(
				cos(m_segments[i].direction.horizAngle() - M_PI/2), 
				0.0f, 
				sin(m_segments[i].direction.horizAngle() - M_PI/2))
			)
		);

		//Set the normal per vertex if a linear curve
		if(curveInterp == GrassStalk::LINEAR){
			m_segments[i].width = baseWidth - (baseWidth * (i+1)/(numWidths));
		}

		m_vertices[i] = newVertex;
	}

	
	//Vec3 min_startColour(0.0f,0.251f,0.167f);
	//Vec3 min_endColour(0.395f,0.551f,0.257f);

	//Vec3 m_min_startColour(0.0f,0.251f,0.167f);
	//Vec3 m_max_startColour(0.275f,0.243f,0.043f);
	//Vec3 m_min_endColour(0.395f,0.551f,0.257f);
	//Vec3 m_max_endColour(0.611f,0.587,0.257f);

	Vec3 startColour = Vec3::lerpColour(colourScale, m_min_startColour, m_max_startColour);
	Vec3 endColour = Vec3::lerpColour(colourScale, m_min_endColour, m_max_endColour);

	//std::cout << "Colour r:" << m_segments[i].colour.x << " g:" << m_segments[i].colour.y << " z:" << m_segments[i].colour.z << std::endl;



	//Handle widths and normals seperately if we're using a cubic curve
	if(curveInterp == GrassStalk::CUBIC){
		for(i = 0; i < numWidths; i++){
			m_segments[i].colour = Vec3::lerpColour((i+1)/numWidths, startColour, endColour);
			//std::cout << "Colour r:" << m_segments[i].colour.x << " g:" << m_segments[i].colour.y << " z:" << m_segments[i].colour.z << std::endl;

			m_segments[i].width = baseWidth - (baseWidth * (i+1)/(numWidths));
			m_segments[i].normal = Vec3::normal( m_segments[i].direction, Vec3(cos(m_segments[i].direction.horizAngle() - M_PI/2), 0.0f, sin(m_segments[i].direction.horizAngle() - M_PI/2) ));
		}
	}
}

float* GrassStalk::getCubicNormalComponents()
{
	float* components = new float[ getNumCubicSegments() * 3 ];

	for (int i = 0; i < getNumCubicSegments(); i++)
	{
		components[i*3] =  m_segments[i].normal.x ;
		components[(i*3)+1] = m_segments[i].normal.y ;
		components[(i*3)+2] = m_segments[i].normal.z ;
	}

	return components;
}

float* GrassStalk::getCubicColourComponents()
{
	float* components = new float[ getNumCubicSegments() * 3 ];

	for (int i = 0; i < getNumCubicSegments(); i++)
	{
		components[i*3] =  m_segments[i].colour.x ;
		components[(i*3)+1] = m_segments[i].colour.y ;
		components[(i*3)+2] = m_segments[i].colour.z ;
	}

	return components;
}


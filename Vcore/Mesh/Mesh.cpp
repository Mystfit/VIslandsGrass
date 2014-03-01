//
//  Mesh.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Mesh.h"

using namespace Vcore;

Mesh::Mesh()
{
}

Mesh::Mesh(Vec3 position)
{
	setPosition(position);
}

float* Mesh::getVertexComponents( Mesh *mesh )
{
	float* components = new float[ (mesh->getTotalVertices() * 3) ];

	for (int i = 0; i < mesh->getTotalVertices(); i++)
	{
		components[i*3] =  mesh->getVertex(i)->x ; 
		components[(i*3)+1] = mesh->getVertex(i)->y ;
		components[(i*3)+2] = mesh->getVertex(i)->z ;
	}

	return components;
}

float* Mesh::getColourComponents( Mesh *mesh )
{
	float* components = new float[ (mesh->getTotalVertices() * 3) ];

	for (int i = 0; i < mesh->getTotalVertices(); i++)
	{
		components[i*3] =  mesh->getVertex(i)->getColour(Vertex::COL_R) ;
		components[(i*3)+1] = mesh->getVertex(i)->getColour(Vertex::COL_G) ;
		components[(i*3)+2] = mesh->getVertex(i)->getColour(Vertex::COL_B) ;
		//std::cout << "Colour Vector x:" << components[i*3] << " y:" << components[(i*3)+1] << " z:" << components[(i*3)+2] << std::endl;

	}

	return components;
}

float* Mesh::getNormalComponents( Mesh *mesh )
{
	float* components = new float[ (mesh->getTotalVertices() * 3) ];

	for (int i = 0; i < mesh->getTotalVertices(); i++)
	{
		components[i*3] =  mesh->getVertex(i)->getNormal().x ;
		components[(i*3)+1] = mesh->getVertex(i)->getNormal().y ;
		components[(i*3)+2] = mesh->getVertex(i)->getNormal().z ;
	}

	return components;
}


float* Mesh::getUVComponents( Mesh *mesh )
{
	float* components = new float[ mesh->getTotalVertices() * 2];

	for (int i = 0; i<mesh->getTotalVertices(); i++)
	{
		components[i*2] = mesh->getVertex(i)->u();
		components[(i*2)+1] = mesh->getVertex(i)->v();
	}

	return components;
}

int* Mesh::getIndices( Mesh *mesh )
{
	int* indices = new int[ mesh->getTotalFaces() * 3 ];
	int i;

	for ( i = 0; i < mesh->getTotalFaces(); i++ )
	{
		int avoidDoubles[3] = {0,0,0};
		int cVertex;

		for ( int j = 0; j < mesh->getTotalVertices(); j++ )
		{
			if ( mesh->getFace(i)->getVertex(0) == mesh->getVertex(j))
			{
				if( avoidDoubles[0] > 0 ){
				} else {
					indices[i*3] = j;
					avoidDoubles[0] = 1;
				}
			}

			else if ( mesh->getFace(i)->getVertex(1) == mesh->getVertex(j))
			{
				if( avoidDoubles[1] > 0 ){
				} else {
					indices[(i*3)+1] = j;
					avoidDoubles[1] = 1;
				}
			}

			else if ( mesh->getFace(i)->getVertex(2) == mesh->getVertex(j))
			{
				if( avoidDoubles[2] > 0 ){
				} else {
					indices[(i*3)+2] = j;
					avoidDoubles[2] = 1;
				}
			}

			else if ( avoidDoubles[0] > 0 && avoidDoubles[1] > 0 && avoidDoubles[2] > 0 )
			{
				break;
			}
		}
	}

	return indices;
}

void Mesh::addFace( Face* face )
{
	m_faces.push_back( face );
}

void Mesh::addVertex( Vertex* vertex )
{
	m_vertices.push_back( vertex );
}

void Mesh::updateNormals()
{
	for (int i = 0; i < m_faces.size(); i++)
		m_faces[i]->calcFaceNormal();
}

void Mesh::clearVertices()
{
	m_vertices.clear();
}

void Mesh::clearFaces()
{
	m_faces.clear();
}



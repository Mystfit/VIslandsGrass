//
//  Mesh.h
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef MESH_H
#define MESH_H 1

#include <vector>
#include <iostream>
#include "Face.h"
#include "Vertex.h"
#include "Vec3.h"

namespace Vcore {
    
    class Mesh 
    {
    public:
        Mesh();
        Mesh(Vec3 position);


        //----------------------
        // Accessors
        //----------------------
        int getTotalFaces()						// Returns total number of faces constructing this mesh
        {
        	return m_faces.size();
        }

        int getTotalVertices()
        {
        	return m_vertices.size();
        }

        Face* getFace( int faceindex )			// Returns a specific face ref 	
        {
		return m_faces[ faceindex ];
        }			
        
        Vertex* getVertex( int vertexindex )	// Returns specific vertex ref
        {
	       return m_vertices[ vertexindex ];
        }	

        Vec3 getPosition()
        {
                return m_position;
        }

        void setPosition(Vec3 position)
        {
                m_position = position;
        }

        static float* getVertexComponents( Mesh *mesh );
        static float* getColourComponents( Mesh *mesh );
        static float* getNormalComponents( Mesh *mesh );
        static float* getUVComponents( Mesh *mesh );

        static int* getIndices( Mesh *mesh );
        

        //----------------------
        //Mesh utils
        //----------------------
        void createVertex(float x);
        void createFace();
        void addFace( Face* face );				// Adds a face pointer to the list. Faces can share vertices
        void addVertex( Vertex* vertex );				// Adds a face pointer to the list. Faces can share vertices
        void updateNormals();
        void clearVertices();
        void clearFaces();


    protected:
    	std::vector<Vertex*> m_vertices;		// Vertice refs
        std::vector<Face*> m_faces; 			// Face draw order refs
        Vec3 m_position;
    };
}

#endif
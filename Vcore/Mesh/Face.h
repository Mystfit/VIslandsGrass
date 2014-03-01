//
//  Face.h
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef FACE_H
#define FACE_H 1

#include "Vertex.h"
#include <iostream>

namespace Vcore {
    
    class Face 
    {
    public:
        Face( Vertex* a, Vertex* b, Vertex* c );

        void calcFaceNormal();

        //------------------------
        // Accessors
        //------------------------
        Vertex* getVertex( int vertexNum )
        {
        	return m_faceVertices[ vertexNum ];
        }

        Vec3 getFaceNormal()
        {
            return  m_faceNormal;
        }    	

        static Vec3 pointOnFace(Face* face, float distX, float distY);
        static float getAveragedHeight(Face* face);   
        static Vec3 getFaceMidPoint(Face* face);   

    protected:
        //Member variables
        Vertex* m_faceVertices [3];
        Vec3 m_faceNormal;
        

        virtual void dummy()        //Present so class is marked as polymorphic
        {
            std::cout << "Inside dummy(). Never run me!" << std::endl;
        }  
    };
}

#endif
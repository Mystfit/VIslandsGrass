//
//  TerrainFace.h
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef TERRAINFACE_H
#define TERRAINFACE_H 1

#include <vector>
#include <iostream>

#include "./Mesh/Vertex.h"
#include "./Mesh/Vec3.h"
#include "./Mesh/Face.h"

namespace Vcore {
    
    class TerrainFace : public Face 
    {
    public:
        TerrainFace(Vertex* a, Vertex* b, Vertex* c);
        static Vec3 getRandomPoint(Face* face, float distX, float distY);

       
    
    protected:
        virtual void dummy()        //Need to overide virutal function in abstract face class
        {
            std::cout << "Inside dummy(). Never run me!" << std::endl;
        }       

        static float randf();
    };
}

#endif
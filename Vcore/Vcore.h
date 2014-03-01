//
//  Vcore.h
//  Vcore
//
//  Created by Byron Mallett on 3/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef VCORE_H
#define VCORE_H 1

#include "Plane.h"
#include <math.h>

#define CURVE_VSTEP 1;

namespace Vcore {
    
    class Visland
    {
    public:
        Visland();
        
        //Build method
        bool build();






        //--------------------------------
        // Getters & setters
        //--------------------------------

        BoundingBox getBoundingBox()
        {
            return m_boundingBox;
        }

        BoundingBox getBoundingBox() const
        {
            return m_boundingBox;
        }

        BoundingBox getPlaneBoundingBox(int planeIndex){
            return m_planeList[planeIndex].getBoundingBox();
        };
        
        //Sets member variables
        void setPlaneParameters (
            int planeSize, 
            int planeHeight, 
            int seed, 
            float roughness, 
            int resolution, 
            int gridSize, 
            int grassMultiplier, 
            float baseWidth,
            float grassSegmentLength,
            int numGrassSegments,
            Vec3 windDirection,
            float grassBendAmount,
            float windSpread,
            int clock,
            int smoothing,
            Vec3 baseColour1,
            Vec3 tipColour1,
            Vec3 baseColour2,
            Vec3 tipColour2
        );
        
        //Retrieve arrays of all components that come back in an x,y,z format
        float* getComponents( int componentType );
        float* getPlaneComponents( int componentType );
        float* getUVs();

        enum componentTypes { VERTICES = 0, COLOURS, NORMALS, UV, GRASS_VERTICES };

        //List of all indices in landscape
        int* getAllIndices();

        //Return individual plane indices
        int* getPlaneIndices(int planeIndex);

        std::vector<Vec3> getAllGeometryInstances();

        //Gets total number of individual faces in the landscape
        int getTotalFaces(){
            int totalFaces = 0;

            for (int i = 0; i < m_gridSize * m_gridSize; i++ )
                totalFaces += m_planeList[i].getTotalFaces();

            return totalFaces;
        };

        Plane& getGrid(int gridIndex)
        {
            return m_planeList[gridIndex];
        }

        //Gets total number of individual vertices in the landscape
        int getTotalVertices()
        {
            int totalVertices = 0;

            for (int i = 0; i < m_gridSize * m_gridSize; i++ )
                totalVertices += m_planeList[i].getTotalVertices();
            
            return totalVertices;
        };

        int getTotalGrassVerts()
        {
            int totalGrassVerts = 0;

            for (int i = 0; i < m_gridSize * m_gridSize; i++ )
                totalGrassVerts += m_planeList[i].getNumGeomInstances() * m_planeList[i].getNumGrassSegments();
            
            return totalGrassVerts;
        };

        int getNumGrids()
        {
            return m_gridSize * m_gridSize;
        }


        
    private:
        //All planes making up landscape
        std::vector<Plane> m_planeList;
        std::vector<Plane> m_grassHeightList;


        //Overall bounding box
        BoundingBox m_boundingBox;

        int m_clock;
        Vec3 m_windDir, m_grassBaseColour1, m_grassTipColour1,  m_grassBaseColour2, m_grassTipColour2;

        int m_planeSize_all, m_planeHeight_all, m_resolution_all, m_seed_all, m_gridSize, m_instanceMultiplier, m_numGrassSegments, m_smoothingAll;
        float m_roughness_all, m_grassSegmentLength, m_grassBendAmount, m_windSpread, m_baseWidth;
    };
};

#endif
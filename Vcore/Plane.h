//
//  Plane.h
//  Vcore
//
//  Created by Byron Mallett on 3/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#ifndef PLANE_H
#define PLANE_H 1

#include "./Mesh/Mesh.h"
#include "./Mesh/Vec3.h"
#include "./Mesh/BoundingBox.h"
#include "TerrainFace.h"
#include "GrassStalk.h"
#include <vector>
#include <math.h>
#include <iostream>

namespace Vcore {
    struct GrassInfo
    {
        float height;
        Vec3 colour;
    };
    
    class Plane : public Mesh
    {
    public:
        
        //Constructors
        Plane();
        
        //Creates plane of fixed size
        bool buildPlane();
        void buildFaces();
        void generateHeightmap();
        void updateBoundingBox();
        std::vector<Vec3> generateWindVelocities(Vec3 windDir,  int clock);
        void generateInstances();

        static void smoothHeightmap(std::vector<Vertex*> v, int kernelSize, int edgeLength);


        //------------------------------------
        // Accessors
        //------------------------------------
        void setOffset( int offsetX, int offsetZ)
        {
            m_offset_x = offsetX;
            m_offset_z = offsetZ;
        }

        int getPlaneSize ()
        {
            return m_size;
        }
        
        void setSeed ( int seed_val )
        {
            m_seed = seed_val;
        }
        
        void setRoughness ( float rough_val)
        {
            m_roughness = rough_val;
        }

        void setResolution ( int size_val )
        {
            m_size = size_val + 1;
            m_realSize = size_val;
        }
        
        void setHeight ( int height_val )
        {
            m_height = height_val;
        }
        
        int getHeight()
        {
            return m_height;
        }

        float getMaxHeight()
        {
            return m_maxHeight;
        }

        float getMinHeight()
        {
            return m_minHeight;
        }
        
        int setScale( int scale)
        {
            m_scale = scale;
        }

        int getScale()
        {
            return m_scale;
        }

        Vec3 getWindVector( int index )
        {
            return m_windVelocityField[index];
        }

        void setGrassParameters(int mult, float baseWidth, float grassSegmentLength, int numGrassSegments, Vec3 windDirection, float grassBendAmount, float windSpread, std::vector<float> grassColourScale, int clock)
        {
            std::cout << "Setter num colour scales:" << grassColourScale.size() << std::endl;
            m_instanceMultiplier = mult;
            m_grassSegmentLength = grassSegmentLength;
            m_numGrassSegments = numGrassSegments;
            m_windDir = windDirection;
            m_grassBendAmount = grassBendAmount;
            m_windSpread = windSpread;
            m_baseWidth = baseWidth;
            m_grassColourScales = grassColourScale;
            m_clock = clock;
        }

        void setGrassColours(Vec3 baseColour1, Vec3 tipColour1, Vec3 baseColour2, Vec3 tipColour2)
        {
            m_grassBaseColour1 = baseColour1;
            m_grassBaseColour2 = baseColour2;
            m_grassTipColour1 = tipColour1;
            m_grassTipColour2 = tipColour2;
        }

        int getNumGrassSegments()
        {
            return m_numGrassSegments;
        }    

        int getTotalGrassSegments()
        {
            return m_totalGrassSegments;
        }    

        int getTotalGrassCubicSegments()
        {
            return m_totalGrassCubicSegments;
        }

        enum planeType {HEIGHTMAP = 0, GRASSCOLOURMAP};
        void setType(int type){
            m_type = type;
        }

        int getType(){
            return m_type;
        }





        void setSmoothStrength(int strength){
            m_kernelSize = (strength * 2) + 1;
        }
        
        BoundingBox getBoundingBox()
        {
            return m_boundingBox;
        }
        
        BoundingBox getBoundingBox() const
        {
            return m_boundingBox;
        }

        TerrainFace* getFaceAsTerrainFace(int index)
        {
            return dynamic_cast<TerrainFace*>(m_faces[index]);
        }



        //----------------------------------------------------
        // Special Accessors (Not just getting/setting values)
        //----------------------------------------------------
        Vertex* getPlaneVertex(int row, int column)
        {
            return getVertex( (row * m_size ) + column );
        }

        Face* getPlaneFace(int row, int column)
        {
            return getFace( (row * m_realSize ) + column );
        }

        static int get2DIndex(int x, int y, int length)
        {
            if(x < 0) x *= -1;
            if(x >= length) x = 2 * length - x - 2; 
            
            if(y < 0) y *= -1;
            if(y >= length) y = 2 * length - y - 2; 

            return (x*length) + y;
        }

        void setPlaneVertex(Vertex* vertex, int row, int column)
        {
            m_vertices[ (row * m_size ) + column ] = vertex;
        }

        std::vector<Vertex*> getEdgeVertices( int side );

        void setEdgeVertices( std::vector<Vertex*>, int side );

        bool leftEdgeSet, upEdgeSet, rightEdgeSet, downEdgeSet;
       

        enum sideLabels { LEFT = 0, UP, RIGHT, DOWN };


        //------------
        // Aggregators
        //------------
        int getNumGeomInstances(){
            return m_geomInstances.size();
        }

        GrassStalk* getGeomInstance(int index)
        {
            return &(m_geomInstances[index]);
        }

        float* getGrassComponents(int componentType);     
        float* getGrassCubicNormalComponents();   
        float* getGrassColours();
        float* getGrassWidths();

        std::vector<float> getGrassColourPlane();

                
        
    private:
        //Generates a random height between -1.0 and 1.0
        float randHeight();
        
        //Size values
        int m_size, 
            m_realSize, 
            m_scale, 
            m_height, 
            m_seed, 
            m_offset_x, 
            m_offset_z, 
            m_instanceMultiplier, 
            m_kernelSize, 
            m_numGrassSegments, 
            m_totalGrassSegments,
            m_totalGrassCubicSegments,
            m_grassType;

        float m_roughness, 
            m_maxHeight, 
            m_minHeight, 
            m_grassSegmentLength, 
            m_grassBendAmount, 
            m_windSpread,
            m_baseWidth;

        Vec3 m_windDir, m_grassBaseColour1, m_grassBaseColour2, m_grassTipColour1, m_grassTipColour2;

        int m_clock, m_type;

        //Grass vars
        std::vector<GrassStalk> m_geomInstances;
        std::vector<GrassInfo> m_grassInfo;
        std::vector<float> m_grassColourScales;
        std::vector<Vec3> m_windVelocityField;
        //Vec3 m_minColour;
        //Vec3 m_maxColour;

        //Bounding box
        BoundingBox m_boundingBox;
        
        //Average shape points methods
        float avgSquareVals (int i, int j, int stride);
        float avgDiamondVals ( int i, int j, int stride, int subSize);
        void setColourToHeight(Vertex* vertex, float height, float scaleHeight);
    };

    
}

#endif
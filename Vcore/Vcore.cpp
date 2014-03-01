//
//  core.cpp
//  core
//
//  Created by Byron Mallett on 3/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Vcore.h"


using namespace Vcore;

//Constructor
Visland::Visland()
{
    //Default plane parameters
    m_seed_all = 1;
    m_planeSize_all = 20;
    m_planeHeight_all = 5;
    m_instanceMultiplier = 10;
    m_resolution_all = 3;
    m_roughness_all = 0.8;
    m_gridSize = 3;
    
    std::cout << "Creating Island..." << std::endl;
}

//Build function - called each time system needs updating
bool Visland::build()
{
    std::cout << "Build called on island" << std::endl;

    m_planeList.clear();            //Clear list of existing planes. TODO-Call destructors on planes first

    float maxHeight;
    float minHeight;
    int target;


    for (int i = 0; i < m_gridSize; i++)
    {
        for (int j = 0; j < m_gridSize; j++)
        {
            //-----------------
            //Build grass colours
            std::cout << "Creating grass colours i:" << i << ", j:" << j << std::endl;

            Plane grassColours;
            Plane newPlane;
            
            grassColours.setType(Plane::GRASSCOLOURMAP);
            grassColours.setOffset( i * m_planeSize_all, j * m_planeSize_all );
            grassColours.setScale( m_planeSize_all );
            grassColours.setHeight( m_planeHeight_all );
            grassColours.setResolution( pow(2, m_resolution_all) );
            grassColours.setSeed( (m_seed_all+1) + (i * m_gridSize) + j );
            grassColours.setRoughness( m_roughness_all );
            grassColours.setSmoothStrength(m_smoothingAll);

            

            //Build terrain
            grassColours.buildPlane();

            

            //Sets edge vertices of adjacent planes so heightmaps will generate using existing height data
            if(i > 0){
                target = ((i-1) * m_gridSize) + j;
                grassColours.setEdgeVertices( m_grassHeightList[ target ].getEdgeVertices(Plane::DOWN), Plane::UP );
            }
            if(j > 0){
                target = (i * m_gridSize) + (j-1);
                grassColours.setEdgeVertices( m_grassHeightList[ target ].getEdgeVertices(Plane::RIGHT), Plane::LEFT );
            }
            
            grassColours.buildFaces();
            grassColours.generateHeightmap();    //Generate plane heightmap   
            grassColours.updateNormals();             
            grassColours.updateBoundingBox();
            

            //-----------------
            //Build Plane

            std::cout << "Creating Plane i:" << i << ", j:" << j << std::endl;

            newPlane.setType(Plane::HEIGHTMAP);
            newPlane.setOffset( i * m_planeSize_all, j * m_planeSize_all );
            newPlane.setScale( m_planeSize_all );
            newPlane.setHeight( m_planeHeight_all );
            newPlane.setResolution( pow(2, m_resolution_all) );
            newPlane.setSeed( m_seed_all + (i * m_gridSize) + j );
            newPlane.setRoughness( m_roughness_all );
            newPlane.setSmoothStrength(m_smoothingAll);

            newPlane.setGrassColours(m_grassBaseColour1, m_grassTipColour1, m_grassBaseColour2, m_grassTipColour2);

            newPlane.setGrassParameters(
                m_instanceMultiplier, 
                m_baseWidth,
                m_grassSegmentLength,
                m_numGrassSegments,
                m_windDir,
                m_grassBendAmount,
                m_windSpread,
                grassColours.getGrassColourPlane(),
                m_clock
            );

            m_grassHeightList.push_back(grassColours);
            

            std::vector<float> testVec;


            //Build terrain
            newPlane.buildPlane();


            //Sets edge vertices of adjacent planes so heightmaps will generate using existing height data
            if(i > 0){
                target = ((i-1) * m_gridSize) + j;
                newPlane.setEdgeVertices( m_planeList[ target ].getEdgeVertices(Plane::DOWN), Plane::UP );
            }
            if(j > 0){
                target = (i * m_gridSize) + (j-1);
                newPlane.setEdgeVertices( m_planeList[ target ].getEdgeVertices(Plane::RIGHT), Plane::LEFT );
            }
            
            newPlane.buildFaces();
            newPlane.generateHeightmap();    //Generate plane heightmap   
            newPlane.updateNormals();        //Calculate plane normals now that we have vertex positions
            newPlane.updateBoundingBox();
            maxHeight = std::max(newPlane.getBoundingBox().getTopBackRight_w().y, maxHeight);
            minHeight = std::min(newPlane.getBoundingBox().getBottomFrontLeft_w().y, minHeight);

            //Build grass
            newPlane.generateWindVelocities( m_windDir, m_clock );
            newPlane.generateInstances();    //Generates new instanced geometry. 

            m_planeList.push_back(newPlane);
        }
    }   
    
    m_boundingBox.setWidth( m_planeSize_all * m_gridSize );
    m_boundingBox.setDepth( m_planeSize_all * m_gridSize );
    m_boundingBox.setHeight( maxHeight - minHeight );
    m_boundingBox.setPosition ( Vec3( m_boundingBox.getWidth() * 0.5, (maxHeight + minHeight) / 2, m_boundingBox.getDepth() * 0.5 ) );
    
    return true;
}


void Visland::setPlaneParameters( 
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
            Vec3 tipColour2)
{
    m_planeSize_all = planeSize;
    m_planeHeight_all =  planeHeight;
    m_resolution_all = resolution;
    m_seed_all = seed;
    m_roughness_all = roughness;
    m_smoothingAll = smoothing;
    m_gridSize = gridSize;
    m_instanceMultiplier = grassMultiplier;
    m_grassSegmentLength = grassSegmentLength;
    m_numGrassSegments = numGrassSegments;
    m_windDir = windDirection;
    m_grassBendAmount = grassBendAmount;
    m_windSpread = windSpread;
    m_baseWidth = baseWidth;
    m_clock = clock;
    m_grassBaseColour1 = baseColour1;
    m_grassTipColour1 = tipColour1;
    m_grassBaseColour2 = baseColour2;
    m_grassTipColour2 = tipColour2;
}


//Gets components from the mesh in x,y,z formats
float* Visland::getComponents( int componentType )
{
    int i,j,k;
    float* components;

    if(componentType != GRASS_VERTICES)
        components = new float[ getTotalVertices() * 3 ];

    float* planeComponents;

    for ( i = 0; i < m_gridSize * m_gridSize; i++)
    {
        int totalVertComponents = m_planeList[i].getTotalVertices() * 3;

        if( componentType == VERTICES){
            planeComponents = Vcore::Mesh::getVertexComponents( &m_planeList[i] );
        }else if( componentType == COLOURS ){
            planeComponents = Vcore::Mesh::getColourComponents( &m_planeList[i] );
        }else if( componentType == NORMALS ){
            planeComponents = Vcore::Mesh::getNormalComponents( &m_planeList[i] );
        }else if( componentType == GRASS_VERTICES )
        {
            planeComponents = m_planeList[i].getGrassComponents(VERTICES);
            int totalGrassComponents = m_planeList[i].getTotalGrassSegments() * 3;

            components = new float[ totalGrassComponents ];
           
            for ( j = 0; j < totalGrassComponents; j++ )
                components[i * totalGrassComponents + j] = planeComponents[j];
        }

        if(componentType != GRASS_VERTICES)
        {
            for ( j = 0; j < totalVertComponents; j++ ){
                components[i * totalVertComponents + j] = planeComponents[j];
            }
        }
    }

    return components;
}

//Gets components from the mesh in x,y,z formats
float* Visland::getUVs()
{
    float* components = new float[ getTotalVertices() * 2 ];
    float* planeComponents;

    float gridUoffset;
    float gridhOffset;

    for ( int i = 0; i < m_gridSize * m_gridSize; i++)
    {
        planeComponents = Vcore::Mesh::getUVComponents( &m_planeList[i] );
        gridUoffset = i/m_gridSize;

        for ( int j = 0; j < m_planeList[i].getTotalVertices(); j++ ){
            gridhOffset = j/m_gridSize;

            components[i*2] = planeComponents[j*2] * gridUoffset;
            components[(i*2)+1] = planeComponents[(j*2)+1] * gridhOffset;

            std::cout << "Generated Ucoord: " << (planeComponents[j*2] * gridUoffset) << " Vcoord: " << (planeComponents[(j*2)+1] * gridhOffset) << std::endl;
        }
    }

    return components;
}


//Combine indices from every plane together so we can render them in one pass
int* Visland::getAllIndices()
{
    int* indices = new int[ getTotalFaces() * 3 ];
    int* planeIndices;

    for ( int i = 0; i < m_gridSize * m_gridSize; i++)
    {
        planeIndices = Vcore::Mesh::getIndices( &m_planeList[i] );

        int totalIndices = m_planeList[i].getTotalFaces() * 3;

        for ( int j = 0; j < totalIndices; j++ )
        {
            indices[i * totalIndices + j] = planeIndices[j] + ( i * m_planeList[i].getTotalVertices() ) ;
        }
    }

    return indices;
}


//Return only the indices from one plane. Useful in renderman for rendering seperate chunks
int* Visland::getPlaneIndices(int planeIndex)
{
    return Vcore::Mesh::getIndices( &m_planeList[planeIndex] );
}


//Get all the seed points from every face in every plane
std::vector<Vec3> Visland::getAllGeometryInstances()
{
    std::vector<Vec3> seedPoints;
    int i, j;
    for ( i = 0; i < m_planeList.size(); i++ )
    {
        for ( j = 0; j < m_planeList[i].getNumGeomInstances(); j++ )
        {            
            seedPoints.push_back( m_planeList[i].getGeomInstance(j)->getPosition() );
        }
    }

    return seedPoints;
}





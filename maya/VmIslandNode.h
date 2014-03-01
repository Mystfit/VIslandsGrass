//
//  VmIslandNode.h
//  maya
//
//  Created by Byron Mallett on 3/14/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef VMISLANDNODE_H
#define VMISLANDNODE_H

#include <maya/MObject.h> 
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MPxLocatorNode.h> 

#include "../Vcore/Vcore.h" 

//-------------------------------------------------------------
//Main object class
//
//Extends the locator node in order to inherit a transform node
//-------------------------------------------------------------

class VmIslandNode : public MPxLocatorNode
{
public:
    //Constructor
    VmIslandNode();
    virtual ~VmIslandNode();
    
    //Maya required functions for id and name of node
    static MTypeId id;
    static MString typeName;
    
    //Creator method to return pointer of this class
    static void* creator();
    static MStatus initialize();
    
    //Computation method - updates attributes and node state
    virtual MStatus compute( const MPlug& i_plug, MDataBlock& i_dataBlock );
    
    //Draw method
    virtual void draw(M3dView& i_view, const MDagPath& i_style, M3dView::DisplayStyle i_style, M3dView::DisplayStatus i_status);
    
    //Bounding boxes
    virtual bool isBounded() const;
    virtual MBoundingBox boundingBox() const;

    
    //Custom Attributes
    //---------------------------------------
    
    //Landscape attributes
    static MObject ia_seed;
    static MObject ia_roughness;
    static MObject ia_planeHeight;
    static MObject ia_smooth;
    static MObject ia_resolution;
    static MObject ia_rmanResolution;
    static MObject ia_planeSize;
    static MObject ia_gridSize;

    //Grass Attributes
    static MObject ia_grassMultiplier;
    static MObject ia_grassSegmentLength;
    static MObject ia_baseWidth;
    static MObject ia_grassNumSegments;
    static MObject ia_windDirection;
    static MObject ia_windSpread;
    static MObject ia_grassBendAmount;

    static MObject ia_grassBaseColour1;
    static MObject ia_grassTipColour1;
    static MObject ia_grassBaseColour2;
    static MObject ia_grassTipColour2;

    static MObject ia_clock;

    //Display attributes
    static MObject ia_drawStyle;

    //IO attributes
    static MObject oa_update;
    static MObject oa_rib;
    
private:
    //Member variables    
    Vcore::Visland m_island;
    
    int m_gridSize;
    float* m_cachedVertices; 
    float* m_cachedColours;
    float* m_cachedNormals;
    int* m_cachedIndices;
    std::vector<Vcore::Vec3> m_geomInstances;

    int m_displayNormalsLength;
        
    //Member methods - draft
    void drawPlane();
    void drawTerrainNormals();
    void drawSubBB();
    void drawBB();
    void drawCube(float bX, float bY, float bZ, float tX, float tY, float tZ);
    void drawInstances();
    void drawWindVelocities();




};

#endif
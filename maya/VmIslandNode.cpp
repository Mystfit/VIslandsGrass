//
//  VmIslandNode.cpp
//  maya
//
//  Created by Byron Mallett on 3/14/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "VmIslandNode.h"

#include <maya/MGlobal.h>
#include <maya/MTypeId.h>
#include <maya/MFloatVector.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnStringData.h>


using namespace Vcore;

//Name and id for node
MTypeId VmIslandNode::id( 0x000723ff );
MString VmIslandNode::typeName( "VmIslandNode" );


//Attribute declarations
MObject VmIslandNode::ia_seed;
MObject VmIslandNode::ia_roughness;
MObject VmIslandNode::ia_planeHeight;
MObject VmIslandNode::ia_smooth;
MObject VmIslandNode::ia_resolution;
MObject VmIslandNode::ia_rmanResolution;
MObject VmIslandNode::ia_planeSize;
MObject VmIslandNode::ia_gridSize; 

MObject VmIslandNode::ia_grassMultiplier; 
MObject VmIslandNode::ia_baseWidth; 
MObject VmIslandNode::ia_grassSegmentLength;
MObject VmIslandNode::ia_grassNumSegments;
MObject VmIslandNode::ia_grassBendAmount;
MObject VmIslandNode::ia_windDirection;
MObject VmIslandNode::ia_windSpread;
MObject VmIslandNode::ia_clock;

MObject VmIslandNode::ia_grassBaseColour1;
MObject VmIslandNode::ia_grassTipColour1;
MObject VmIslandNode::ia_grassBaseColour2;
MObject VmIslandNode::ia_grassTipColour2;

MObject VmIslandNode::ia_drawStyle;
MObject VmIslandNode::oa_update;
MObject VmIslandNode::oa_rib;


//Constructor/deconstructor
//Sets up member variables but not attributes - see the initialize() method
void* VmIslandNode::creator()
{
    return (void*)( new VmIslandNode );
}


VmIslandNode::VmIslandNode()
{
}


VmIslandNode::~VmIslandNode()
{
}


//=====================================================
// Compute method
//
// The compute method gets called whenever data is needed
// from this class. When data is needed, a plug accessing
// the attribute in question is asked for data, and that
// might result in a call to this method. The data being
// asked for is given by the i_plug argument. All the
// data, as it exists at the time of calling this method,
// is available from the io_dataBlock. Notice that data is
// taken from the data-block, and not from plugs.
//-----------------------------------------------------

MStatus VmIslandNode::compute( const MPlug& i_plug, MDataBlock& io_dataBlock )
{
    MStatus status;
    
    fprintf( stderr, "VmIslandNode::compute()...\n" );
    
    //Check plugs
    if( i_plug == oa_update )
    {
        //Make sure all internal structures are up to date with attributes
        fprintf( stderr, "VmIslandNode::compute(oa_update)\n" );
        
        MDataHandle seedHandle = io_dataBlock.inputValue( ia_seed, & status);
        const long seed = seedHandle.asLong();
        CHECK_MSTATUS( status );
        
        MDataHandle roughnessHandle = io_dataBlock.inputValue( ia_roughness, & status);
        const float roughness = roughnessHandle.asFloat();
        CHECK_MSTATUS( status );
        
        MDataHandle planeHeightHandle = io_dataBlock.inputValue( ia_planeHeight, & status);
        const long planeHeight = planeHeightHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle smoothHandle = io_dataBlock.inputValue( ia_smooth, & status);
        const long smooth = smoothHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle resolutionHandle = io_dataBlock.inputValue( ia_resolution, & status);
        const long resolution = resolutionHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle planeSizeHandle = io_dataBlock.inputValue( ia_planeSize, & status);
        const long planeSize = planeSizeHandle.asLong();
        CHECK_MSTATUS( status );


        
        MDataHandle gridSizeHandle = io_dataBlock.inputValue( ia_gridSize, & status);
        const long gridSize = gridSizeHandle.asLong();
        m_gridSize = (int) gridSize;
        CHECK_MSTATUS( status );


        //Grass
        //--------------
        MDataHandle baseWidthHandle = io_dataBlock.inputValue( ia_baseWidth, & status);
        const float baseWidth = baseWidthHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle grassMultiplierHandle = io_dataBlock.inputValue( ia_grassMultiplier, & status);
        const long grassMultiplier = grassMultiplierHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle grassSegmentLengthHandle = io_dataBlock.inputValue( ia_grassSegmentLength, & status);
        const float grassSegmentLength = grassSegmentLengthHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle grassNumSegmentsHandle = io_dataBlock.inputValue( ia_grassNumSegments, & status);
        const long grassNumSegments = grassNumSegmentsHandle.asLong();
        CHECK_MSTATUS( status );

        //MDataHandle windDirectionHandle = io_dataBlock.inputValue( ia_windDirection, & status);
        MFloatVector& windDirVec = io_dataBlock.inputValue( ia_windDirection, & status).asFloatVector();
        const Vcore::Vec3 windDirection = Vec3(windDirVec.x, windDirVec.y, windDirVec.z);
        CHECK_MSTATUS( status );

        MDataHandle grassBendAmountHandle = io_dataBlock.inputValue( ia_grassBendAmount, & status);
        const float grassBendAmount = grassBendAmountHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle windSpreadHandle = io_dataBlock.inputValue( ia_windSpread, & status);
        const float windSpread = windSpreadHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle clockHandle = io_dataBlock.inputValue( ia_clock, & status);
        const float clock = clockHandle.asLong();
        CHECK_MSTATUS( status );

        //Colours
        //-----------------
        MFloatVector& grassBaseColour1Vec = io_dataBlock.inputValue( ia_grassBaseColour1, & status).asFloatVector();
        const Vcore::Vec3 grassBaseColour1 = Vec3(grassBaseColour1Vec.x, grassBaseColour1Vec.y, grassBaseColour1Vec.z);
        CHECK_MSTATUS( status );

        MFloatVector& grassTipColour1Vec = io_dataBlock.inputValue( ia_grassTipColour1, & status).asFloatVector();
        const Vcore::Vec3 grassTipColour1 = Vec3(grassTipColour1Vec.x, grassTipColour1Vec.y, grassTipColour1Vec.z);
        CHECK_MSTATUS( status );

        MFloatVector& grassBaseColour2Vec = io_dataBlock.inputValue( ia_grassBaseColour2, & status).asFloatVector();
        const Vcore::Vec3 grassBaseColour2 = Vec3(grassBaseColour2Vec.x, grassBaseColour2Vec.y, grassBaseColour2Vec.z);
        CHECK_MSTATUS( status );

        MFloatVector& grassTipColour2Vec = io_dataBlock.inputValue( ia_grassTipColour2, & status).asFloatVector();
        const Vcore::Vec3 grassTipColour2 = Vec3(grassTipColour2Vec.x, grassTipColour2Vec.y, grassTipColour2Vec.z);
        CHECK_MSTATUS( status );


        //Update paramters of external lib system and rebuild
        m_island.setPlaneParameters( 
            planeSize, 
            planeHeight, 
            seed, 
            roughness, 
            resolution, 
            gridSize, 
            grassMultiplier, 
            baseWidth,
            grassSegmentLength, 
            grassNumSegments, 
            windDirection, 
            grassBendAmount, 
            windSpread, 
            clock, 
            smooth,
            grassBaseColour1,
            grassTipColour1,
            grassBaseColour2,
            grassTipColour2
        );
        
        //Rebuild object and check for success
        const bool updateOK = m_island.build();

        m_cachedVertices = m_island.getComponents( Vcore::Visland::VERTICES );
        m_cachedColours = m_island.getComponents( Vcore::Visland::COLOURS );
        m_cachedNormals = m_island.getComponents( Vcore::Visland::NORMALS );
        m_cachedIndices = m_island.getAllIndices();
        m_geomInstances = m_island.getAllGeometryInstances();
        
        // We must set a value for the plug we have been asked to evaluate,
        // even if we are not going to use it. We set it in the data-block,
        // and to set it we use outputValue().
        //
        // Here we usually set the result to true. The caller who triggered the
        // computation for this attribute might not look at the value that
        // we are setting this plug to. But they do ask for the value of this plug,
        // only to trigger an update of the internal structures. See the draw()
        // and boundingBox() methods to see how this is done.
        
        MDataHandle updateHandle = io_dataBlock.outputValue( i_plug );
        updateHandle.set( updateOK );
        
        //Need to set plug to clean to refresh it
        io_dataBlock.setClean( i_plug );
    }
    
    else if( i_plug == oa_rib ) {
        //Set up rib system to ready renderman values
        
        fprintf( stderr, "VmIslandNode::compute(oa_rib)\n" );
        
        MDataHandle seedHandle = io_dataBlock.inputValue( ia_seed, & status);
        const long seed = seedHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle smoothHandle = io_dataBlock.inputValue( ia_smooth, & status);
        const float smooth = smoothHandle.asLong();
        CHECK_MSTATUS( status );
        
        MDataHandle roughnessHandle = io_dataBlock.inputValue( ia_roughness, & status);
        const float roughness = roughnessHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle rmanResolutionHandle = io_dataBlock.inputValue( ia_rmanResolution, & status);
        const long rmanResolution = rmanResolutionHandle.asLong();
        CHECK_MSTATUS( status );
        
        MDataHandle planeHeightHandle = io_dataBlock.inputValue( ia_planeHeight, & status);
        const long planeHeight = planeHeightHandle.asLong();
        CHECK_MSTATUS( status );
        
        MDataHandle planeSizeHandle = io_dataBlock.inputValue( ia_planeSize, & status);
        const long planeSize = planeSizeHandle.asLong();
        CHECK_MSTATUS( status );


        //Grass
        //----------------

        MDataHandle gridSizeHandle = io_dataBlock.inputValue( ia_gridSize, & status);
        const long gridSize = gridSizeHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle grassMultiplierHandle = io_dataBlock.inputValue( ia_grassMultiplier, & status);
        const long grassMultiplier = grassMultiplierHandle.asLong();
        CHECK_MSTATUS( status );

        MDataHandle baseWidthHandle = io_dataBlock.inputValue( ia_baseWidth, & status);
        const float baseWidth = baseWidthHandle.asFloat();
        m_gridSize = (int) baseWidth;
        CHECK_MSTATUS( status );

        MDataHandle grassSegmentLengthHandle = io_dataBlock.inputValue( ia_grassSegmentLength, & status);
        const float grassSegmentLength = grassSegmentLengthHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle grassNumSegmentsHandle = io_dataBlock.inputValue( ia_grassNumSegments, & status);
        const long grassNumSegments = grassNumSegmentsHandle.asLong();
        CHECK_MSTATUS( status );

        MFloatVector& windDirVec = io_dataBlock.inputValue( ia_windDirection, & status).asFloatVector();
        const Vcore::Vec3 windDirection = Vec3(windDirVec.x, windDirVec.y, windDirVec.z);
        CHECK_MSTATUS( status );

        MDataHandle grassBendAmountHandle = io_dataBlock.inputValue( ia_grassBendAmount, & status);
        const float grassBendAmount = grassBendAmountHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle windSpreadHandle = io_dataBlock.inputValue( ia_windSpread, & status);
        const float windSpread = windSpreadHandle.asFloat();
        CHECK_MSTATUS( status );

        MDataHandle clockHandle = io_dataBlock.inputValue( ia_clock, & status);
        const float clock = clockHandle.asLong();
        CHECK_MSTATUS( status );

        //Colours
        //-------------------
        MFloatVector& grassBaseColour1Vec = io_dataBlock.inputValue( ia_grassBaseColour1, & status).asFloatVector();
        const Vcore::Vec3 grassBaseColour1 = Vec3(grassBaseColour1Vec.x, grassBaseColour1Vec.y, grassBaseColour1Vec.z);
        CHECK_MSTATUS( status );

        MFloatVector& grassTipColour1Vec = io_dataBlock.inputValue( ia_grassTipColour1, & status).asFloatVector();
        const Vcore::Vec3 grassTipColour1 = Vec3(grassTipColour1Vec.x, grassTipColour1Vec.y, grassTipColour1Vec.z);
        CHECK_MSTATUS( status );

        MFloatVector& grassBaseColour2Vec = io_dataBlock.inputValue( ia_grassBaseColour2, & status).asFloatVector();
        const Vcore::Vec3 grassBaseColour2 = Vec3(grassBaseColour2Vec.x, grassBaseColour2Vec.y, grassBaseColour2Vec.z);
        CHECK_MSTATUS( status );

        MFloatVector& grassTipColour2Vec = io_dataBlock.inputValue( ia_grassTipColour2, & status).asFloatVector();
        const Vcore::Vec3 grassTipColour2 = Vec3(grassTipColour2Vec.x, grassTipColour2Vec.y, grassTipColour2Vec.z);
        CHECK_MSTATUS( status );

        char rib[4096];
        sprintf( rib, "seed=%d;roughness=%f;planeHeight=%d;planeSize=%d;resolution=%d;gridSize=%d;grassMultiplier=%d;baseWidth=%f;grassSegmentLength=%f;grassNumSegments=%d;windDirectionX=%f;windDirectionY=%f;windDirectionZ=%f;grassBendAmount=%f;windSpread=%f;clock=%d;smooth=%d;grassBaseColour1X=%f;grassBaseColour1Y=%f;grassBaseColour1Z=%f;grassTipColour1X=%f;grassTipColour1Y=%f;grassTipColour1Z=%f;grassBaseColour2X=%f;grassBaseColour2Y=%f;grassBaseColour2Z=%f;grassTipColour2X=%f;grassTipColour2Y=%f;grassTipColour2Z=%f;",
            (int) seed, 
            (float) roughness, 
            (int) planeHeight, 
            (int) planeSize, 
            (int) rmanResolution , 
            (int) gridSize, 
            (int) grassMultiplier, 
            (float) baseWidth,
            (float) grassSegmentLength, 
            (int) grassNumSegments, 
            (float) windDirection.x, 
            (float) windDirection.y,
            (float) windDirection.z,
            (float) grassBendAmount, 
            (float) windSpread,
            (int) clock,
            (int) smooth,
            (float) grassBaseColour1.x,
            (float) grassBaseColour1.y,
            (float) grassBaseColour1.z,
            (float) grassTipColour1.x,
            (float) grassTipColour1.y,
            (float) grassTipColour1.z,
            (float) grassBaseColour2.x,
            (float) grassBaseColour2.y,
            (float) grassBaseColour2.z,
            (float) grassTipColour2.x,
            (float) grassTipColour2.y,
            (float) grassTipColour2.z
        );        
        
        // We must set a value for the plug we have been asked to evaluate,
        // even if we are not going to use it. We set it in the data-block,
        // and to set it we use outputValue().
        //
        // Here we are calculating a string value, and that value will 
        // be used by the caller in a "rib-gen" operation - a process in 
        // which a RIB file is generated for Renderman.
        //
        // Notice also that strings in Maya or more complicated than numbers.
        // We need to make a data object for the string data.
        
        MFnStringData stringDataFn;
        MObject stringDataObj = stringDataFn.create( rib, & status );
        CHECK_MSTATUS( status );
        
        MDataHandle ribHandle = io_dataBlock.outputValue( i_plug );
        ribHandle.set( stringDataObj );
        io_dataBlock.setClean( i_plug );
    }
    
    else {
        //Shouldn't be here.
        return MStatus::kSuccess;
    }
}



//=====================================================
//Bounding boxes
//Whether or not bounding box is set
bool VmIslandNode::isBounded() const
{
    return true;
}

MBoundingBox VmIslandNode::boundingBox() const
{
    MPlug updatePlug( thisMObject(), VmIslandNode::oa_update );
    const bool updateOK = updatePlug.asBool();
    if ( updateOK == false )
    {
        MGlobal::displayError( "VmIslandNode::boundingBox() - update failed\n" );
    }
    
    MBoundingBox boundingBox( 
         MPoint( m_island.getBoundingBox().getBottomFrontLeft_w().x,
            m_island.getBoundingBox().getBottomFrontLeft_w().y,
            m_island.getBoundingBox().getBottomFrontLeft_w().z ), 
         MPoint( m_island.getBoundingBox().getTopBackRight_w().x,
            m_island.getBoundingBox().getTopBackRight_w().y,
            m_island.getBoundingBox().getTopBackRight_w().z )
         );
        
    return boundingBox;
}
 



//=====================================================
// Return the bounding box. This function is one way we
// make everything up to date - because it's from within
// this function that we asked for the node to be updated
// by asking for the value of oa_update. Same happens
// in draw(). 
//
// Note - The draw() method will NEVER GET CALLED if we are
// running Maya without the interface (i.e. primpt mode),
// which is something that is often done. But, the 
// boundingBox() method WILL get called. We ask for the
// value of oa_update in both of these methods to make
// sure all structures are up to date.
//-----------------------------------------------------



//Draw Plane helper
void VmIslandNode::drawPlane()
{
    int numFaces = m_island.getTotalFaces() * 3;

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);  //sets lighting to one-sided
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, m_cachedNormals);
    glColorPointer(3, GL_FLOAT, 0, m_cachedColours);
    glVertexPointer(3, GL_FLOAT, 0, m_cachedVertices);

    glDrawElements(GL_TRIANGLES, numFaces, GL_UNSIGNED_INT, m_cachedIndices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glDisable(GL_LIGHTING);
}

void VmIslandNode::drawTerrainNormals()
{
    Vec3 faceNormal;
    Vec3 faceMid;
    Vec3 drawVec;
    Vec3 slopeVec;


    glBegin( GL_LINES );

    for(int i = 0; i < m_island.getNumGrids(); i++)
    {
        for(int j = 0; j < m_island.getGrid(i).getTotalFaces(); j++)
        {
            faceNormal = m_island.getGrid(i).getFace(j)->getFaceNormal().normalize(0.2f);
            faceMid = Vcore::Face::getFaceMidPoint(m_island.getGrid(i).getFace(j));
            
            drawVec = (faceMid + faceNormal);
            slopeVec = (faceMid + Vcore::Vec3::normal(faceNormal, 
                Vec3(
                    cos(faceNormal.horizAngle() + 3.14159f/2), 
                    0.0f, 
                    sin(faceNormal.horizAngle() + 3.14159f/2 ))
            ).normalize(0.2f));

            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f( faceMid.x, faceMid.y, faceMid.z );
            glVertex3f( drawVec.x, drawVec.y,drawVec.z ); 

            glColor3f(1.0f,1.0f,0.0f);
            glVertex3f( faceMid.x, faceMid.y, faceMid.z );
            glVertex3f( slopeVec.x, slopeVec.y,slopeVec.z ); 
        }
    }

    glEnd();
}


//Draw lines to represent vertex normals
void VmIslandNode::drawInstances()
{
    /*
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_island.getComponents(Vcore::Visland::VERTICES));
    glDrawArrays(GL_LINES, 0, m_island.getTotalGrassVerts());
    glDisableClientState(GL_VERTEX_ARRAY);
    */

    GrassStalk * grassStalk;
    Vec3 curveVec;
    Vec3 curveNormal;

    glBegin( GL_LINES );

    for(int i = 0; i < m_island.getNumGrids(); i++)
    {
        for(int j = 0; j < m_island.getGrid(i).getNumGeomInstances(); j++)
        {
            grassStalk = m_island.getGrid(i).getGeomInstance(j);

            for(int k = 0; k < m_island.getGrid(i).getNumGrassSegments()-1; k++){
                curveVec = *(grassStalk->getVertex(k));
                curveNormal = curveVec + grassStalk->getVertex(k)->getNormal().normalize(0.2f);

                //Draw segment
                glColor3f(0.05f,0.1f,0.6f);
                glVertex3f( grassStalk->getVertex(k)->x, grassStalk->getVertex(k)->y, grassStalk->getVertex(k)->z );
                glVertex3f( grassStalk->getVertex(k+1)->x, grassStalk->getVertex(k+1)->y, grassStalk->getVertex(k+1)->z );

                //Draw segment normal
                glColor3f(0.2f,0.4f,1.0f);
                glVertex3f( grassStalk->getVertex(k)->x, grassStalk->getVertex(k)->y, grassStalk->getVertex(k)->z );
                glVertex3f( curveNormal.x, curveNormal.y, curveNormal.z );
            }
        }
    }
    glEnd();
}


void VmIslandNode::drawWindVelocities()
{
    Vec3 faceMid;
    Vec3 windVector;
    Vec3 drawVec;

    glBegin( GL_LINES );

    for(int i = 0; i < m_island.getNumGrids(); i++)
    {
        for(int j = 0; j < m_island.getGrid(i).getTotalFaces()/2; j++)
        {
            windVector = m_island.getGrid(i).getWindVector(j);
            faceMid = Vcore::Face::getFaceMidPoint(m_island.getGrid(i).getFace(j*2));
            //faceMid = Vcore::Face::getFaceMidPoint(m_island.getGrid(i).getFace(j)) + (Vcore::Face::getFaceMidPoint(m_island.getGrid(i).getFace(j+1)) - Vcore::Face::getFaceMidPoint(m_island.getGrid(i).getFace(j)));
            drawVec = faceMid + windVector;

            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f( faceMid.x, faceMid.y*0.5f, faceMid.z );
            glVertex3f( drawVec.x, drawVec.y*0.5f ,drawVec.z ); 

        }
    }

    glEnd();
}

void VmIslandNode::drawCube(float bX, float bY, float bZ, float tX, float tY, float tZ)
{
    glBegin( GL_LINES );

    glVertex3f( bX, bY, bZ );
    glVertex3f( tX, bY, bZ );

    glVertex3f( bX, bY, bZ );
    glVertex3f( bX, tY, bZ );

    glVertex3f( bX, bY, bZ );
    glVertex3f( bX, bY, tZ );

    glVertex3f( tX, tY, tZ );
    glVertex3f( bX, tY, tZ );

    glVertex3f( tX, tY, tZ );
    glVertex3f( tX, bY, tZ );

    glVertex3f( tX, tY, tZ );
    glVertex3f( tX, tY, bZ );

    glVertex3f( tX, bY, bZ );
    glVertex3f( tX, bY, tZ );

    glVertex3f( tX, bY, bZ );
    glVertex3f( tX, tY, bZ );

    glVertex3f( bX, tY, bZ );
    glVertex3f( tX, tY, bZ );

    glVertex3f( bX, tY, bZ );
    glVertex3f( bX, tY, tZ );

    glVertex3f( bX, bY, tZ );
    glVertex3f( tX, bY, tZ);

    glVertex3f( bX, bY, tZ );
    glVertex3f( bX, tY, tZ );


    glEnd();
}

void VmIslandNode::drawBB()
{
    drawCube(m_island.getBoundingBox().getBottomFrontLeft_w().x,
        m_island.getBoundingBox().getBottomFrontLeft_w().y, 
        m_island.getBoundingBox().getBottomFrontLeft_w().z, 
        m_island.getBoundingBox().getTopBackRight_w().x, 
        m_island.getBoundingBox().getTopBackRight_w().y, 
        m_island.getBoundingBox().getTopBackRight_w().z
    );
}


//Draw cube helper - good for bounding box;
void VmIslandNode::drawSubBB()
{
    for(int i = 0; i < m_gridSize * m_gridSize; i++){

        drawCube(m_island.getPlaneBoundingBox(i).getBottomFrontLeft_w().x,
            m_island.getPlaneBoundingBox(i).getBottomFrontLeft_w().y, 
            m_island.getPlaneBoundingBox(i).getBottomFrontLeft_w().z, 
            m_island.getPlaneBoundingBox(i).getTopBackRight_w().x, 
            m_island.getPlaneBoundingBox(i).getTopBackRight_w().y, 
            m_island.getPlaneBoundingBox(i).getTopBackRight_w().z
        );
    }
}


//=====================================================
// Drawing function. The drawing function is one way we
// make everything up to date - because it's from within
// this function that we asked for the node to be updated
// by asking for the value of oa_update. The same thing
// happens in boundingBox().
//
// Note - The draw() method will NEVER GET CALLED if we are
// running Maya without the interface (i.e. primpt mode),
// which is something that is often done. But, the 
// boundingBox() method WILL get called. We ask for the
// value of oa_update in both of these methods to make
// sure all structures are up to date.
//-----------------------------------------------------

void VmIslandNode::draw( M3dView& i_view, const MDagPath& i_path, M3dView::DisplayStyle i_style, M3dView::DisplayStatus i_status )
{
    //fprintf( stderr, "VmIslandNode::Draw()\n" );
    
    //Start OpenGL
    i_view.beginGL();
    
    // Get all the values we need from the plugs.
    //
    // Note that for the oa_update plug, we don't actually
    // care about the value we get from it. But we do ask
    // for it to be computed, since that computation process
    // will get the internal structures up to date. But, the
    // drawStyle() plug we do care about the value of. At the
    // same time though, in the compute() method, nothing
    // needs to be computed for it.
    
    MPlug updatePlug( thisMObject(), VmIslandNode::oa_update );
    const bool updateOK = updatePlug.asBool();
    if( updateOK == false)
    {
        MGlobal::displayError( "VmIslandNode::draw() - update failed\n" );
    }
    
    MPlug drawStylePlug( thisMObject(), VmIslandNode::ia_drawStyle );
    const int drawStyle = drawStylePlug.asInt();
    
    //Check plugs
    if (drawStyle == 0){        
        drawBB();
    } else if (drawStyle == 1){        
        drawSubBB();
    } else if ( drawStyle == 2 ){
        drawPlane();
    } else if( drawStyle == 3)  {
        drawPlane();
        drawTerrainNormals();
    }else if (drawStyle == 4 ) {
        drawPlane();
        drawInstances();        
    } else if (drawStyle == 5 ) {
        drawPlane();
        drawInstances();
        drawWindVelocities();
    } else if (drawStyle == 6 ) {
        drawPlane();
        drawBB();
        drawSubBB();
        drawInstances();
        drawTerrainNormals();
        drawWindVelocities();
    }
        
    
    //Close OpenGL
    i_view.endGL();
}




//====================================================================
//Initialize function - Creates attributes and describes relationships
//--------------------------------------------------------------------

MStatus VmIslandNode::initialize()
{
    fprintf( stderr, "VmIslandNode::initialize()...\n" );
    
    MStatus status;
    
    //Seed attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_seed = numericAttrFn.create( "seed", "sD", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0 );
        numericAttrFn.setMax( 1000 );
        numericAttrFn.setDefault ( 0 );
        status = addAttribute( ia_seed );
        CHECK_MSTATUS( status );
    }
    
    //Roughness attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_roughness = numericAttrFn.create( "roughness", "rG", MFnNumericData::kFloat, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0 );
        numericAttrFn.setMax( 1 );
        numericAttrFn.setDefault ( 0.75 );
        status = addAttribute( ia_roughness );
        CHECK_MSTATUS( status );
    }
    
    //Plane Height attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_planeHeight = numericAttrFn.create( "planeHeight", "pH", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0 );
        numericAttrFn.setMax( 2000 );
        numericAttrFn.setDefault ( 5 );
        status = addAttribute( ia_planeHeight );
        CHECK_MSTATUS( status );
    }

     //Plane smoothing attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_smooth = numericAttrFn.create( "smoothingStrength", "sS", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0 );
        numericAttrFn.setMax( 6 );
        numericAttrFn.setDefault ( 1 );
        status = addAttribute( ia_smooth );
        CHECK_MSTATUS( status );
    }
    
     //Plane resolution attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_resolution = numericAttrFn.create( "mayaResolution", "mR", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 1 );
        numericAttrFn.setMax( 10 );
        numericAttrFn.setDefault ( 1 );
        status = addAttribute( ia_resolution );
        CHECK_MSTATUS( status );
    }

    //Renderman resolution attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_rmanResolution = numericAttrFn.create( "rendermanResolution", "rR", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 1 );
        numericAttrFn.setMax( 10 );
        numericAttrFn.setDefault ( 6 );
        status = addAttribute( ia_rmanResolution );
        CHECK_MSTATUS( status );
    }

    //Plane size attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_planeSize = numericAttrFn.create( "planeSizeScale", "pS", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 1 );
        numericAttrFn.setMax( 200 );
        numericAttrFn.setDefault ( 20 );
        status = addAttribute( ia_planeSize );
        CHECK_MSTATUS( status );
    }


     //Plane size attribute
    {
        MFnNumericAttribute numericAttrFn;
        ia_gridSize = numericAttrFn.create( "gridSize", "gS", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 1 );
        numericAttrFn.setMax( 10 );
        numericAttrFn.setDefault ( 1 );
        status = addAttribute( ia_gridSize );
        CHECK_MSTATUS( status );
    }

     //Grass multiplier - Affects how many instances are spawned
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassMultiplier = numericAttrFn.create( "grassInstanceMultiplier", "gM", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 1 );
        numericAttrFn.setMax( 100 );
        numericAttrFn.setDefault ( 1 );
        status = addAttribute( ia_grassMultiplier );
        CHECK_MSTATUS( status );
    }


        //Grass multiplier - Affects how many instances are spawned
    {
        MFnNumericAttribute numericAttrFn;
        ia_baseWidth = numericAttrFn.create( "grassBaseWidth", "bW", MFnNumericData::kFloat, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0.01f );
        numericAttrFn.setMax( 2.0f ); 
        numericAttrFn.setDefault ( 0.6f );
        status = addAttribute( ia_baseWidth );
        CHECK_MSTATUS( status );
    }



    //Grass segment length - Length of segment pieces
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassSegmentLength = numericAttrFn.create( "grassSegmentLength", "gSL", MFnNumericData::kFloat, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0.001f );
        numericAttrFn.setMax( 10.0f );
        numericAttrFn.setDefault ( 1.0f );
        status = addAttribute( ia_grassSegmentLength );
        CHECK_MSTATUS( status );
    }

     //Number of segments per piece of grass
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassNumSegments = numericAttrFn.create( "numberOfGrassSegments", "nGS", MFnNumericData::kLong, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 3 );
        numericAttrFn.setMax( 20 );
        numericAttrFn.setDefault ( 5 );
        status = addAttribute( ia_grassNumSegments );
        CHECK_MSTATUS( status );
    }

    //Initial grass bend direction
    {
        MFnNumericAttribute numericAttrFn;
        ia_windDirection = numericAttrFn.createPoint( "windDirection", "wDir");
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setDefault(1.0f, 0.0f, 1.0f);
        status = addAttribute( ia_windDirection );
        CHECK_MSTATUS( status );
    }

    //Grass bend factor
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassBendAmount = numericAttrFn.create( "grassBendFactor", "gBF", MFnNumericData::kFloat, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0.0f );
        numericAttrFn.setMax( 1.0f );
        numericAttrFn.setDefault ( 0.0f );
        status = addAttribute( ia_grassBendAmount );
        CHECK_MSTATUS( status );
    }

    //Wind strength
    {
        MFnNumericAttribute numericAttrFn;
        ia_windSpread = numericAttrFn.create( "windSpread", "wS", MFnNumericData::kFloat, 0, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0.01f );
        numericAttrFn.setMax( 10.0f );
        numericAttrFn.setDefault ( 5.0f );
        status = addAttribute( ia_windSpread );
        CHECK_MSTATUS( status );
    }



        //Initial grass bend direction
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassBaseColour1 = numericAttrFn.createColor( "grassBaseColour1", "bCol1");
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setDefault(0.0f,0.251f,0.167f);
        status = addAttribute( ia_grassBaseColour1 );
        CHECK_MSTATUS( status );
    }

            //Initial grass bend direction
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassTipColour1 = numericAttrFn.createColor( "grassTipColour1", "tCol1");
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setDefault(0.395f,0.551f,0.257f);
        status = addAttribute( ia_grassTipColour1 );
        CHECK_MSTATUS( status );
    }

            //Initial grass bend direction
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassBaseColour2 = numericAttrFn.createColor( "grassBaseColour2", "bCol2");
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setDefault(0.275f,0.243f,0.043f);
        status = addAttribute( ia_grassBaseColour2 );
        CHECK_MSTATUS( status );
    }

            //Initial grass bend direction
    {
        MFnNumericAttribute numericAttrFn;
        ia_grassTipColour2 = numericAttrFn.createColor( "grassTipColour2", "tCol2");
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( true );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setDefault(0.611f,0.587f,0.257f);
        status = addAttribute( ia_grassTipColour2 );
        CHECK_MSTATUS( status );
    }





     //Clock attribute. Passes maya frame counter into node
    {
        MFnNumericAttribute numericAttrFn;
        ia_clock = numericAttrFn.create( "clock", "clk", MFnNumericData::kLong, false, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( false );
        numericAttrFn.setKeyable( true );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( false );
        numericAttrFn.setMin( 0 );
        numericAttrFn.setDefault( 0 );
        status = addAttribute( ia_clock );
        CHECK_MSTATUS( status );
    }

    
    // "drawStyle"
    //
    // How we should draw in Maya.
    //
    // We will use this value in our Maya plug-in, but we
    // will NOT use it in Renderman. Note elsewhere that
    // this value will not be sent to Renderman, nor will
    // it have any effect on RIB generation.
    //
    // We think of this as an input attribute - hence the
    // prefix "ia_" for "input attribute". 
    {
        MFnEnumAttribute enumAttrFn;
        ia_drawStyle = enumAttrFn.create( "drawStyle", "ds", 1, & status );
        CHECK_MSTATUS( status );
        enumAttrFn.setReadable( true );
        enumAttrFn.setWritable( true );
        enumAttrFn.setStorable( true );
        enumAttrFn.setKeyable( true );
        enumAttrFn.setConnectable( true );
        enumAttrFn.setHidden( false );
        enumAttrFn.addField( "Bounds", 0 );
        enumAttrFn.addField( "Sub bounds", 1 );
        enumAttrFn.addField( "Geometry", 2 );
        enumAttrFn.addField( "Terrain slope normals", 3 );
        enumAttrFn.addField( "Point instances", 4 );
        enumAttrFn.addField( "Wind Velocity", 5 );
        enumAttrFn.addField( "All", 6 );
        enumAttrFn.setDefault(3);
        status = addAttribute( ia_drawStyle );
        CHECK_MSTATUS( status );
    }
    
    // "update"
    //
    // A 'dummy' attribute, and a powerful one. Although
    // the value of this attribute is actually meaningless,
    // when we ask for it we trigger an important computation
    // processs.
    //
    // We think of this as an input attribute - hence the
    // prefix "oa_" for "computation attribute". This signifies
    // that we don't really care about it's final value, but
    // that we know it's going to compute a lot of stuff. 
    {
        MFnNumericAttribute numericAttrFn;
        oa_update = numericAttrFn.create( "update", "upd", MFnNumericData::kBoolean, false, & status );
        CHECK_MSTATUS( status );
        numericAttrFn.setReadable( true );
        numericAttrFn.setWritable( true );
        numericAttrFn.setStorable( false );
        numericAttrFn.setKeyable( false );
        numericAttrFn.setConnectable( true );
        numericAttrFn.setHidden( true );
        status = addAttribute( oa_update );
        CHECK_MSTATUS( status );
    }


   
    
    
    // "rib"
    //
    // A string which contains all the attributes, and that
    //  we need to have (and indeed will see again) on the 
    // Renderman side of things.
    //
    // We think of this as an output attribute - hence the
    // prefix "oa_" for "output attribute". The value
    // computed for this attribute (the string) is important,
    // an is used by whatever is asking for it.
    {
        MFnTypedAttribute typedAttrFn;
        oa_rib = typedAttrFn.create( "rib", "rb", MFnData::kString, MObject::kNullObj, & status );
        CHECK_MSTATUS( status );
        typedAttrFn.setReadable( true );
        typedAttrFn.setWritable( true );
        typedAttrFn.setStorable( false );
        typedAttrFn.setKeyable( false );
        typedAttrFn.setConnectable( true );
        typedAttrFn.setHidden( true );
        status = addAttribute( oa_rib );
        CHECK_MSTATUS( status );
    }


    
    // This section tells Maya what attribute effects
    // which attribute. When input attributes change, 
    // make the attributes they effect "dirty". That 
    // means that, when Maya asks for a dirty attribute, 
    // it will need to be computed and "cleaned".
    //
    // Changing an effecting "input" attribute does not
    // trigger a computation of it's effected "output"
    // attribute. It just tells Maya that - if the effected
    // attribute is ever asked for, it will have to call
    // the compute method to calculate it.
    //
    // For complex relationships between attributes,
    // an attributeEffects() call must exist for each
    // relationship. If A effects B, and B effects C,
    // then we will need to make two attributeEffects()
    // calls - one for A effecting B and one for B 
    // effecting C. Maya will not figure out that, if A
    // effects B, and B effets C, then A must effect C.
    //
    // Below we see that most "input" attributes effect
    // both our internal update and the rib generation
    // the same way in each case. But it's not always 
    // exactly the same like this.
    //
    // Notice also that ia_drawStyle does not effect
    // any of the attributes. It's something that's just
    // used in the draw method.
    
    attributeAffects( ia_seed, oa_update );
    attributeAffects( ia_seed, oa_rib );
    
    attributeAffects( ia_roughness, oa_update );
    attributeAffects( ia_roughness, oa_rib );

    attributeAffects( ia_smooth, oa_update );
    attributeAffects( ia_smooth, oa_rib );

    attributeAffects( ia_planeHeight, oa_update );
    attributeAffects( ia_planeHeight, oa_rib );

    attributeAffects( ia_resolution, oa_update );
    attributeAffects( ia_rmanResolution, oa_rib );
    
    attributeAffects( ia_planeSize, oa_update );
    attributeAffects( ia_planeSize, oa_rib );

    attributeAffects( ia_gridSize, oa_update );
    attributeAffects( ia_gridSize, oa_rib );

    attributeAffects( ia_grassMultiplier, oa_update );
    attributeAffects( ia_grassMultiplier, oa_rib );

    attributeAffects( ia_baseWidth, oa_update );
    attributeAffects( ia_baseWidth, oa_rib );

    attributeAffects( ia_grassSegmentLength, oa_update );
    attributeAffects( ia_grassSegmentLength, oa_rib );

    attributeAffects( ia_grassNumSegments, oa_update );
    attributeAffects( ia_grassNumSegments, oa_rib );

    attributeAffects( ia_grassBendAmount, oa_update );
    attributeAffects( ia_grassBendAmount, oa_rib );

    attributeAffects( ia_windDirection, oa_update );
    attributeAffects( ia_windDirection, oa_rib );

    attributeAffects( ia_windSpread, oa_update );
    attributeAffects( ia_windSpread, oa_rib );

    attributeAffects( ia_clock, oa_update );
    attributeAffects( ia_clock, oa_rib );

    attributeAffects( ia_grassBaseColour1, oa_update);
    attributeAffects( ia_grassBaseColour1, oa_rib);

    attributeAffects( ia_grassTipColour1, oa_update);
    attributeAffects( ia_grassTipColour1, oa_rib);

    attributeAffects( ia_grassBaseColour2, oa_update);
    attributeAffects( ia_grassBaseColour2, oa_rib);

    attributeAffects( ia_grassTipColour2, oa_update);
    attributeAffects( ia_grassTipColour2, oa_rib);


    
    
    fprintf( stderr, "VmIslandNode::initialize() done\n" );

    return MStatus::kSuccess;
}


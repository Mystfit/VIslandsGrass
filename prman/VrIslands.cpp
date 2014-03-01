#include "VrIslands.h"
#include <string>
#include <map>
#include "../Vcore/Vcore.h"

using namespace std;
using namespace Vcore;

//Forwards declarations
void renderGrid( Visland*, const int );

static int s_gridReferenceCount = 0;

//Reference class for subgrid objects
struct Subgrid
{
    Visland* m_visland;
    int m_gridId;
};

//////////////////////////////////////////////////////////
// 
// Not part of Renderman.
//
// Simple parsing routines, to split up a string like this -
//
//	name=value;name=value;name=value;
//
// Into a map of 
//
//	name->value, name->value, name->value 
// 
//////////////////////////////////////////////////////////

static void parseParameters( const char* i_parameterString, map<string,string>& o_parameterMap )
{
    char name[ 4096 ];
    int n = 0;

    char value[ 4096 ];
    int v = 0;

    int state = 0;
    for ( int x = 0; x < (int) strlen( i_parameterString); x++ )
    {
        const char c = i_parameterString[ x ];

        if ( state == 0 )
        {
            if ( c == '=' )
            {
                name[ n ] = '\0';
                n = 0;
                state = 1;
            }
            else
            {
                name[ n++ ] = c;
            }
        }
        else if ( state == 1 )
        {
            if ( c == ';' )
            {
                value[ v ] = '\0';
                v = 0;
                o_parameterMap[ name ] = value;
                state = 0;
            }
            else
            {
                value[ v++ ] = c; 
            }
        }
    }
}

void toFloat( const string& i_valueString, float& o_value )
{
    if ( i_valueString != "" )
    {
        o_value = atof( i_valueString.c_str() );
    }
}

void toInt( const string i_valueString, int& o_value )
{
    if ( i_valueString != "" )
    {
        o_value = atoi( i_valueString.c_str() );
    }
}

void toLong( const string i_valueString, long& o_value )
{
    if ( i_valueString != "" )
    {
        o_value = atol( i_valueString.c_str() );
    }
}

void toBool( const string i_valueString, bool& o_value )
{
    if ( i_valueString != "" )
    {
        o_value = (bool) atoi( i_valueString.c_str() );
    }
}

//////////////////////////////////////////////////////////
// 
// Part of Renderman.
//
// ConvertParameters()
//
// This is your "init" routine.
//
// Convert the incoming parameter string into real values, 
// allocate a core library object with it, and return it to 
// Renderman.
// 
//////////////////////////////////////////////////////////

RtPointer ConvertParameters( RtString i_parameterString )
{
    fprintf( stderr, "ConvertParameters() parameters:%s ...\n",
        i_parameterString );

    //------------------------------------------------
    // Make new example object - based on parameters.

    /*
    Example* example = new Example;

    toInt( parameterMap[ "seed" ].c_str(), example->seed() );
    toInt( parameterMap[ "numClusters" ], example->numClusters() );
    toInt( parameterMap[ "numPointsPerCluster" ], example->numPointsPerCluster() );
    toFloat( parameterMap[ "speed" ], example->speed() );
    toFloat( parameterMap[ "clusterScale" ], example->clusterScale() );
    toFloat( parameterMap[ "pointSize" ], example->pointSize() );
    toBool( parameterMap[ "useMotionBlur" ], example->useMotionBlur() );

    example->build();

    fprintf( stderr, "ConvertParameters() out\n" );

    return (RtPointer) example;
    */

    map<string,string> parameterMap;
    parseParameters( i_parameterString, parameterMap );

    int ribSeed;
    int ribPlaneHeight;
    int ribPlaneSize;
    int ribSmooth;
    int ribResolution;
    int ribGridSize;
    int ribGrassMultiplier;
    float ribBaseWidth;
    float ribGrassSegmentLength;
    int ribGrassNumSegments;
    float ribGrassBendAmount;
    float ribWindSpread;
    int ribClock;
    float ribRoughness;

    Vcore::Vec3 ribWindDirection;
    Vcore::Vec3 ribGrassBaseColour1;
    Vcore::Vec3 ribGrassTipColour1;
    Vcore::Vec3 ribGrassBaseColour2;
    Vcore::Vec3 ribGrassTipColour2;

    Visland* visland = new Visland();
    ++s_gridReferenceCount;


    toInt( parameterMap[ "seed" ].c_str(), ribSeed );
    toInt( parameterMap[ "planeHeight" ].c_str(), ribPlaneHeight );
    toInt( parameterMap[ "planeSize" ].c_str(), ribPlaneSize );
    toInt( parameterMap[ "resolution" ].c_str(), ribResolution );
    toInt( parameterMap[ "gridSize" ].c_str(), ribGridSize );
    toInt( parameterMap[ "grassMultiplier" ].c_str(), ribGrassMultiplier );
    toInt( parameterMap[ "grassNumSegments" ].c_str(), ribGrassNumSegments );
    toInt( parameterMap[ "smooth" ].c_str(), ribSmooth );
    toInt( parameterMap[ "clock" ].c_str(), ribClock );
    toFloat( parameterMap[ "baseWidth" ].c_str(), ribBaseWidth );
    toFloat( parameterMap[ "grassSegmentLength" ].c_str(), ribGrassSegmentLength );
    toFloat( parameterMap[ "windDirectionX" ].c_str(), ribWindDirection.x );
    toFloat( parameterMap[ "windDirectionY" ].c_str(), ribWindDirection.y );
    toFloat( parameterMap[ "windDirectionZ" ].c_str(), ribWindDirection.z );
    toFloat( parameterMap[ "grassBendAmount" ].c_str(), ribGrassBendAmount );
    toFloat( parameterMap[ "windSpread" ].c_str(), ribWindSpread );
    toFloat( parameterMap[ "roughness" ].c_str(), ribRoughness );

    toFloat( parameterMap[ "grassBaseColour1X" ].c_str(), ribGrassBaseColour1.x );
    toFloat( parameterMap[ "grassBaseColour1Y" ].c_str(), ribGrassBaseColour1.y );
    toFloat( parameterMap[ "grassBaseColour1Z" ].c_str(), ribGrassBaseColour1.z );
    toFloat( parameterMap[ "grassTipColour1X" ].c_str(), ribGrassTipColour1.x );
    toFloat( parameterMap[ "grassTipColour1Y" ].c_str(), ribGrassTipColour1.y );
    toFloat( parameterMap[ "grassTipColour1Z" ].c_str(), ribGrassTipColour1.z );
    toFloat( parameterMap[ "grassBaseColour2X" ].c_str(), ribGrassBaseColour2.x );
    toFloat( parameterMap[ "grassBaseColour2Y" ].c_str(), ribGrassBaseColour2.y );
    toFloat( parameterMap[ "grassBaseColour2Z" ].c_str(), ribGrassBaseColour2.z );    
    toFloat( parameterMap[ "grassTipColour2X" ].c_str(), ribGrassTipColour2.x );
    toFloat( parameterMap[ "grassTipColour2Y" ].c_str(), ribGrassTipColour2.y );
    toFloat( parameterMap[ "grassTipColour2Z" ].c_str(), ribGrassTipColour2.z );

  
    //ribGrassNumSegments = 6;

    //std::cout << "Colour test:" << ribGrassTipColour1.x  << std::endl;

    visland->setPlaneParameters(
        ribPlaneSize, 
        ribPlaneHeight, 
        ribSeed, 
        ribRoughness, 
        ribResolution, 
        ribGridSize, 
        ribGrassMultiplier, 
        ribBaseWidth,
        ribGrassSegmentLength, 
        ribGrassNumSegments, 
        ribWindDirection, 
        ribGrassBendAmount, 
        ribWindSpread,
        ribClock,
        ribSmooth,
        ribGrassBaseColour1,
        ribGrassTipColour1,
        ribGrassBaseColour2,
        ribGrassTipColour2
    );

    fprintf( stderr, "ConvertParameters() out\n" );


    visland->build();

    return (RtPointer) visland;

   


}

//////////////////////////////////////////////////////////
// 
// Part of Renderman.
//
// Subdivide()
//
// Your "render" routine.
//
// Cast the incoming data to your core object.
// Use Renderman calls to draw what you need to draw.
// Notice - all this contains is a loop that will get
// the sub objects and call a routine to render them.
// Doing it this way will make it simpler for the next
// phase of the course.
//
// The "detail" parameter tells us how many square pixels
// our bounding box covers in screen space. Ignore it.
// 
//////////////////////////////////////////////////////////

RtVoid Subdivide( RtPointer i_data, RtFloat i_detail )
{
    fprintf( stderr, "Subdivide() data:%p detail:%f ...\n",
        i_data, i_detail );

    Visland* visland = (Visland*) i_data;

    for ( int gridIndex = 0; gridIndex < visland->getNumGrids(); gridIndex++ )
    {
        const BoundingBox& box = visland->getPlaneBoundingBox( gridIndex );
        RtBound bound = {
            box.getBottomFrontLeft_w().x, box.getTopBackRight_w().x,
            box.getBottomFrontLeft_w().y, box.getTopBackRight_w().y,
            box.getBottomFrontLeft_w().z, box.getTopBackRight_w().z
        };

        fprintf( stderr, "Subdivide() %d: [%f %f ,, %f %f ,, %f %f]\n",
            (int) gridIndex,
            bound[ 0 ], bound[ 1 ], bound[ 2 ],
            bound[ 3 ], bound[ 4 ], bound[ 5 ] );

        ++s_gridReferenceCount;

        Subgrid* sub = new Subgrid();
        sub->m_visland = visland;
        sub->m_gridId = gridIndex;

        RiProcedural(
            sub,
            bound,
            Primitive_Subdivide,
            Primitive_Free );

        //renderGrid( visland, gridIndex );
    }

    fprintf( stderr, "Subdivide() done\n" );
}

//////////////////////////////////////////////////////////
//
// Free
// 
// Your "delete" function.
//
// Cast the incoming data to your core object.
// Then delete it.
//
//////////////////////////////////////////////////////////

RtVoid Free( RtPointer i_data )
{
    fprintf( stderr, "Free() data:%p ...\n",
        i_data );

    Visland* visland = (Visland*) i_data; 

    //////////////////////////////////////////////////////
    //
    // We want to delete the core library object ONLY WHEN
    // we know none of the other threads are using it.
    //
    //////////////////////////////////////////////////////

    --s_gridReferenceCount;
    if ( s_gridReferenceCount <= 0 )
    {
        delete visland;
        visland = NULL;
    }
     
    fprintf( stderr, "Free() done\n" );
}

//////////////////////////////////////////////////////////
// 
// renderCluster
//
// This is one of my local functions that renders a 
// single cluster of my blob.
// 
//////////////////////////////////////////////////////////

void Primitive_Subdivide( RtPointer i_data, RtFloat i_detail )
{
    Subgrid* sub = (Subgrid*) i_data;
    Visland* visland = sub->m_visland;
    int gridIndex = sub->m_gridId;

    fprintf( stderr, "render() Terrain:%p Zone ID:%d...\n", visland, gridIndex );  

    //Core instance ref
    Vcore::Plane * grid = &visland->getGrid(gridIndex);

    //Fill an array with number of verts per face (three by default)
    int nVertsPerFace[grid->getTotalFaces()*3];
    std::fill_n(nVertsPerFace, grid->getTotalFaces()*3, 3);

    RtInt * indices =  Vcore::Mesh::getIndices(grid);


    //Curve data
    int numCurves = grid->getNumGeomInstances();

    //Number of vertices per piece of grass
    int nVertsPerCurve[numCurves];
    std::fill_n(nVertsPerCurve, numCurves, grid->getNumGrassSegments());

    /*
    RtFloat * curveWidths = new float[grid->getNumGeomInstances() * grid->getNumGrassSegments()];

    int vStep = 1;
    int uStep = 1;
    int numWidths = ((grid->getNumGrassSegments() - 4) / vStep) + 2;

    int i,j;
    for(i = 0; i < grid->getNumGeomInstances(); i++){
        for(j = 0; j < grid->getGeomInstace(i)->getNumWidths(); j++){
            curveWidths[i * numWidths + j] = 0.6f - (0.6f * (j+1)/(numWidths));
        }
    }
    */
    
    

    std::cout << "Num Grass Instances:" << numCurves << " Num Verts per instance: " << grid->getNumGrassSegments() << std::endl;

    RiAttributeBegin();
        RiPointsPolygons(
            (RtInt) grid->getTotalFaces(), // Total faces in mesh.
            nVertsPerFace, // An array of vertices per faces.
            indices, // The indices of the vertices in the mesh.
            RI_P, Vcore::Mesh::getVertexComponents( grid ), // The vertices in the mesh.
            RI_CS, Vcore::Mesh::getColourComponents( grid ), // The vertices in the mesh.
            RI_N, Vcore::Mesh::getNormalComponents( grid ), // The normals in the mesh.
            RI_ST, Vcore::Mesh::getUVComponents( grid ),    //UV coordinates
            RI_NULL
        );
        RiBasis (RiCatmullRomBasis, 1, RiCatmullRomBasis, 1);
        RiCurves(
            (RtToken) "cubic", 
            (RtInt) numCurves, 
            nVertsPerCurve, 
            RI_NONPERIODIC,
            RI_P, grid->getGrassComponents(Vcore::Visland::VERTICES),
            RI_N, grid->getGrassCubicNormalComponents(),
            RI_CS, grid->getGrassColours(),
            (RtToken) "width", grid->getGrassWidths(), 
            RI_NULL 
        );
        
    RiAttributeEnd();
    

    fprintf( stderr, "Primitive_Subdivide() done\n" );
}


RtVoid Primitive_Free( RtPointer i_data )
{
    fprintf( stderr, "Primitive_Free() data:%p ...\n",
        i_data );

    Subgrid* sub = (Subgrid*) i_data;

    //////////////////////////////////////////////////////
    //
    // We want to delete the core library object ONLY WHEN
    // we know none of the other threads are using it.
    //
    //////////////////////////////////////////////////////

    --s_gridReferenceCount;
    if ( s_gridReferenceCount <= 0 )
    {
        Visland* visland = sub->m_visland;

        delete visland;
        visland = NULL;
    }

    //////////////////////////////////////////////////////
    //
    // Deleting the subExample object will not also delete
    // the items it contains, if they are pointers to 
    // other objects. No fear.
    //
    //////////////////////////////////////////////////////

    delete sub;
    sub = NULL;

    fprintf( stderr, "Primitive_Free() done\n" );
}




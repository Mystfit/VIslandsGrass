//
//  Plane.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Plane.h"
#include "Vcore.h"
#include <vector>

using namespace Vcore;

//Default constructor
Plane::Plane()
{    
    setResolution(4);
    m_height = 1;
    m_roughness = 0.7f;
    m_scale = 100;  
    m_grassType = GrassStalk::CUBIC;
    m_instanceMultiplier = 10;
    m_numGrassSegments = 6;
    m_windDir = Vec3(1.0f,0,1.0f);
    leftEdgeSet = false;
    upEdgeSet = false;
    rightEdgeSet = false;
    downEdgeSet = false;
}



//-----------------------------------
// Landscape methods
//-----------------------------------

//Creates plane of fixed size of vertices
bool Plane::buildPlane() 
{  
    int i,j;
    
    clearVertices();
    

    m_minHeight = 0;
    m_maxHeight = 0;

    //Generate vertices in a grid
    for ( i = 0 ; i < m_size; i++ ) {
        
        for ( j = 0; j < m_size; j++ ) {
            Vertex * point = new Vertex( ((float) i / ( (float)m_size - 1) * m_scale) + m_offset_x , 0.0f, ((float) j / ( (float)m_size - 1) * m_scale ) + m_offset_z );
            point->u(i / m_size);
            point->v(j / m_size);
            addVertex( point );

            //Create wind velocity field
        }
    }

    return true;
}


//Use plane vertices to build face list
void Plane::buildFaces()
{
    clearFaces();

    for (int i = 0; i < m_realSize; i++)
    {
        for (int j = 0; j < m_realSize; j++)
        {
            addFace( new Vcore::Face( getPlaneVertex(i,j), getPlaneVertex(i,j+1), getPlaneVertex(i+1, j) ) ) ;
            addFace( new Vcore::Face( getPlaneVertex(i+1, j), getPlaneVertex(i, j+1), getPlaneVertex(i+1,j+1) ) ); 
        }
    }
}


//Update bounding box dimensions
void Plane::updateBoundingBox()
{
    float newHeight = m_maxHeight - m_minHeight + ((m_maxHeight - m_minHeight) * 0.05) * 20 + 10;

    m_boundingBox.setWidth( m_scale );
    m_boundingBox.setDepth( m_scale );
    m_boundingBox.setHeight( newHeight );
    m_boundingBox.setPosition (
        Vec3( (m_scale * 0.5) + (float) m_offset_x, 
            (m_maxHeight + m_minHeight) / 2.0f, 
            (m_scale * 0.5) + (float) m_offset_z ) 
    );
}


//Get a list of all the vertices along one edge of the plane. Use these to create an adjacent plane
std::vector<Vertex*> Plane::getEdgeVertices( int side )
{
    std::vector<Vertex*> vertices;

    for( int i = 0; i < m_size; i++)
    {
        if( side == LEFT)
            vertices.push_back( getPlaneVertex( i, 0 ) );
        else if( side == UP)
            vertices.push_back( getPlaneVertex( 0, i  ) );
        else if ( side == RIGHT)
            vertices.push_back( getPlaneVertex( i, m_size-1 ) );
        else if (side == DOWN)
            vertices.push_back( getPlaneVertex( m_size-1, i ) );
    }

    return vertices;
}

//Sets edge of plane to existing vertex
void Plane::setEdgeVertices( std::vector<Vertex*> edgeVertices, int side )
{
    for (int i = 0; i < edgeVertices.size(); i++)
    {
        if ( side == LEFT )
        {
            setPlaneVertex( edgeVertices[i], i, 0 );
            leftEdgeSet = true;   
        } else if( side == UP )
        {
            setPlaneVertex( edgeVertices[i], 0, i );
            upEdgeSet = true;
        } else if( side == RIGHT )
        {
            setPlaneVertex( edgeVertices[i], i, edgeVertices.size()-1 );
            rightEdgeSet = true;
        } else if( side == DOWN )
        {
            setPlaneVertex( edgeVertices[i], edgeVertices.size()-1, i );
            downEdgeSet = true;
        }
    }
}




//-----------------------------------
// Grass methods
//-----------------------------------

//Get all of the components for the grass curves
float* Plane::getGrassComponents( int componentType )
{
    float* components = new float[ getNumGeomInstances() * m_numGrassSegments * 3 ];
    float* grassComponents;

    for ( int i = 0; i < getNumGeomInstances(); i++)
    {
        if( componentType == Vcore::Visland::VERTICES)
            grassComponents = Vcore::Mesh::getVertexComponents( getGeomInstance(i) );
        else if( componentType == Vcore::Visland::NORMALS )
            grassComponents = Vcore::Mesh::getNormalComponents( getGeomInstance(i) );

        for ( int j = 0; j < m_numGrassSegments * 3; j++ ){
            components[i * m_numGrassSegments * 3 + j] = grassComponents[j];
        }
    }

    std::cout << getNumGeomInstances() * m_numGrassSegments * 3 << " components. " << getNumGeomInstances() << " grass instances." << std::endl;

    return components;
}

float *Plane::getGrassCubicNormalComponents(){
    float* components = new float[ getNumGeomInstances() * m_numGrassSegments * 3 ];
    float* grassComponents;
    int numComponents;
    int i,j;

    for ( i = 0; i < getNumGeomInstances(); i++)
    {
        grassComponents = getGeomInstance(i)->getCubicNormalComponents();
        numComponents = getGeomInstance(i)->getNumCubicSegments() * 3;

        for ( j = 0; j < numComponents; j++ )
            components[i * numComponents + j] = grassComponents[j];
    }

    return components;
}

float* Plane::getGrassWidths()
{
    float* components = new float[ getNumGeomInstances() * m_numGrassSegments ];

    for (int i = 0; i < getNumGeomInstances(); i++)
    {
        for (int j = 0; j < getGeomInstance(i)->getNumCubicSegments(); j++)
        {
            components[i * getGeomInstance(i)->getNumCubicSegments() + j] = getGeomInstance(i)->getSegmentWidth(j);
            //components[i * m_numGrassSegments + j] = (float)j/(float)m_numGrassSegments;
        }
    }

    return components;
}

float* Plane::getGrassColours()
{
    float* components = new float[ getNumGeomInstances() * m_numGrassSegments * 3 ];
    float* grassComponents;
    int numComponents;
    int i,j;

    for ( i = 0; i < getNumGeomInstances(); i++)
    {
        grassComponents = getGeomInstance(i)->getCubicColourComponents();
        numComponents = getGeomInstance(i)->getNumCubicSegments() * 3;

        for ( j = 0; j < numComponents; j++ )
            components[i * numComponents + j] = grassComponents[j];
    }

    return components;
}



//Creates random instances on the surface of the terrain based on dules. Currently uses height of faces.
void Plane::generateInstances()
{   
    int i,j;
    std::vector<Vec3> windVelocityField = generateWindVelocities(m_windDir, m_clock);
    m_windVelocityField = windVelocityField;

    //std::cout << "INSTANCE CLOCK:" << m_clock << std::endl;
    //std::cout << "WIND VEC 1 x:" << windVelocityField[0].x << " y:" << windVelocityField[0].y << " z:" << windVelocityField[0].z << std::endl;
    //std::cout << "WIND VEC 2 x:" << windVelocityField[1].x << " y:" << windVelocityField[1].y << " z:" << windVelocityField[1].z << std::endl;
    //std::cout << "WIND VEC 3 x:" << windVelocityField[2].x << " y:" << windVelocityField[2].y << " z:" << windVelocityField[2].z << std::endl;
    std::cout << "Plane gen colour r:" << m_grassBaseColour1.x << " g:" << m_grassBaseColour1.y << " z:" << m_grassBaseColour1.z << std::endl;

    for (i = 0; i < m_faces.size(); i++)
    {
        int numInstances = (m_height / (Face::getAveragedHeight(m_faces[i]) + m_height) ) * m_instanceMultiplier;
        //int numInstances = 1;
        for( j = 0; j < numInstances; j++)
        {
            GrassStalk grass( 
                Face::pointOnFace(m_faces[i], 
                ((float)rand()/(float)RAND_MAX), 
                ((float)rand()/(float)RAND_MAX)), 
                //Face::getFaceMidPoint(m_faces[i]),
                m_numGrassSegments, 
                m_grassSegmentLength, 
                m_baseWidth,
                m_grassBendAmount, 
                m_faces[i]->getFaceNormal(), 
                m_clock, 
                windVelocityField[i/2], 
                m_grassType,
                m_grassColourScales[i/2],
                m_grassBaseColour1,
                m_grassBaseColour2,
                m_grassTipColour1,
                m_grassTipColour2
                //m_grassInfo[i/2].height
            );
            m_geomInstances.push_back(grass);
        }
    }
}

//Generate the velocity field for grass waves
std::vector<Vec3> Plane::generateWindVelocities(Vec3 windDir, int clock)
{
    int i,j;
    int fieldWidth = (m_size-1);
    //float maxWidth = (float)m_size * (float)m_scale;
    float maxWidth = ((float)m_size + (float)m_offset_x) * m_scale;
    float maxHeight = ((float)m_size + (float)m_offset_z) * m_scale;
    float maxDist = sqrt(maxWidth*maxWidth + maxHeight*maxHeight);

    float dist;
    float windStrength;
    Vec3 p;
    std::vector<Vec3> windVelocityField;


    //std::cout << "Size is " << fieldWidth << " MaxWidth is " << maxWidth <<std::endl;

    windVelocityField.clear();
    windVelocityField.resize(fieldWidth * fieldWidth);

    for(i = 0; i < fieldWidth; i++)
    {
        for(j=0; j < fieldWidth; j++)
        {
            dist = abs( windDir.z * ((float)i + (float)m_offset_x) * m_scale + windDir.x * ((float)j + (float)m_offset_z) * m_scale ) / sqrt(windDir.z*windDir.z + windDir.x*windDir.x);
            windStrength = sin( (dist/maxDist + (float)clock/25.0f) * m_windSpread )*0.5f+0.5f;

            p.x = sin(windDir.horizAngle()) * windStrength;
            p.z = cos(windDir.horizAngle()) * windStrength;
            windVelocityField[i * fieldWidth + j] = p;
        }
    }

    return windVelocityField;

    //std::cout << "Generated " << (m_size-1)*(m_size-1) << " wind vectors" << std::endl;
}

std::vector<float> Plane::getGrassColourPlane()
{
    std::vector<float> v;

    v.resize(m_faces.size());
    for(int i = 0; i < m_faces.size(); i++){
        v[i] = (m_maxHeight - Face::getFaceMidPoint(m_faces[i]).y) / (m_maxHeight - m_minHeight);
    }
    return v;
}






//-----------------------------------
// Heightmap
//-----------------------------------

//Generates the diamond-square heightmap
void Plane::generateHeightmap()
{
    int i, j, k;
    int stride;
    int oddline;
    int subSize;
    float ratio, scale, height, scaleHeight;
    
    srand( m_seed );            // Initialize the seed for initial plane
    
    subSize = m_realSize;       // Store original plane power size for reference
        
    ratio = (float) pow (2.0f, m_roughness * -1.0f);
    scale = (float) m_height * ratio;   
    scaleHeight = (float) m_height;

    
    //Only set random corners if we aren't sharing any vertices with adjacent planes
    if(!leftEdgeSet && !rightEdgeSet && !upEdgeSet && !downEdgeSet){
        getPlaneVertex( 0, 0 )->y = randHeight() * scale;
        getPlaneVertex( 0, subSize )->y = randHeight() * scale;
        getPlaneVertex( subSize, 0 )->y = randHeight() * scale;
        getPlaneVertex( subSize, subSize )->y = randHeight() * scale;
    }

    if(m_type == HEIGHTMAP){
        getPlaneVertex( 0, 0 )->setVecColour( Vec3::lerpColour(m_grassColourScales[0], m_grassBaseColour1, m_grassBaseColour2) );
        getPlaneVertex( 0, subSize )->setVecColour( Vec3::lerpColour(m_grassColourScales[subSize], m_grassBaseColour1, m_grassBaseColour2) );
        getPlaneVertex( subSize, 0 )->setVecColour( Vec3::lerpColour(m_grassColourScales[(subSize * subSize) - subSize], m_grassBaseColour1, m_grassBaseColour2) );
        getPlaneVertex( subSize, subSize )->setVecColour( Vec3::lerpColour(m_grassColourScales[subSize * subSize], m_grassBaseColour1, m_grassBaseColour2) );
    }

    //setColourToHeight( getPlaneVertex( 0, 0 ), getPlaneVertex( 0, 0 )->y, scaleHeight );
    //setColourToHeight( getPlaneVertex( 0, subSize ), getPlaneVertex( 0, subSize )->y, scaleHeight );
    //setColourToHeight( getPlaneVertex( subSize, 0 ), getPlaneVertex( subSize, 0 )->y, scaleHeight );
    //setColourToHeight( getPlaneVertex( subSize, subSize ), getPlaneVertex( subSize, subSize )->y, scaleHeight );
        
    stride = subSize / 2;       // Set stride as half of real size of plane
   
    //Diamond square algorhithim - Loop through the plane in increasing detail until we've given a random height to every point
    while (stride) 
    {
        //Diamond step - Use squares to generate diamond centerpoints
        for ( i = stride; i < m_size; i+=stride)
        {
            for ( j = stride; j < m_size; j+=stride){
                height = avgSquareVals(i,j,stride) + randHeight() * scale;

                //Set max and min of bounding box based on largest generated value
                m_maxHeight = std::max(m_maxHeight, height);
                m_minHeight = std::min(m_minHeight, height);

                //std::cout << "Num colour scales:" << m_grassColourScales.size() << std::endl;

                getPlaneVertex( i, j )->y = height;

                if(m_type == HEIGHTMAP)
                    getPlaneVertex( i, j )->setVecColour( Vec3::lerpColour(m_grassColourScales[i * subSize + j ], m_grassBaseColour1, m_grassBaseColour2) );

                //setColourToHeight( getPlaneVertex( i, j ), height, scaleHeight );

                j += stride;
            }
            
            i += stride;
        }
        
        //Square step - Use diamonds to generate square midpoints. We need to get every other since we aren't using squares
        oddline = 0;
        
        for ( i = 0; i < m_size; i += stride )
        {
            oddline = (oddline == 0);   //Not sure what this line does
            
            for ( j = 0; j < m_size; j +=stride )
            {
                //Skip a line if we're odd
                if ( ( oddline ) && !j )
                    j += stride;

                height = avgDiamondVals(i, j, stride, subSize) + randHeight() * scale; 

                if( leftEdgeSet && j == 0 ){
                    height = getPlaneVertex( i, j )->y;
                } 
                if( upEdgeSet && i == 0 ) {
                    height = getPlaneVertex( i, j )->y;
                } 
                if( rightEdgeSet && j == m_size-1 ) {
                    height = getPlaneVertex( i, j )->y;
                } 
                if( downEdgeSet && i == m_size-1 ) {
                    height = getPlaneVertex( i, j )->y;
                }


                //Set max and min of bounding box based on largest generated value
                m_maxHeight = std::max(m_maxHeight, height);
                m_minHeight = std::min(m_minHeight, height);

                getPlaneVertex( i, j )->y = height;
                if(m_type == HEIGHTMAP)
                    getPlaneVertex( i, j )->setVecColour( Vec3::lerpColour(m_grassColourScales[i * subSize + j ], m_grassBaseColour1, m_grassBaseColour2) );
                //setColourToHeight( getPlaneVertex( i, j ), height, scaleHeight );

                //Wrap edges
                if ( i == 0){
                    getPlaneVertex( m_size-1, j )->y = getPlaneVertex( i, j )->y;
                    if(m_type == HEIGHTMAP)
                        getPlaneVertex( m_size-1, j )->setVecColour( Vec3::lerpColour(m_grassColourScales[subSize * (subSize-1) + j ], m_grassBaseColour1, m_grassBaseColour2) );
                    //setColourToHeight( getPlaneVertex( m_size-1, j ), height, scaleHeight );
                }
                if ( j == 0) {
                    getPlaneVertex( i, m_size-1 )->y = getPlaneVertex( i, j )->y;
                   // if(m_type == HEIGHTMAP)
                        //getPlaneVertex( i, m_size-1 )->setVecColour( Vec3::lerpColour(m_grassColourScales[i * subSize + (subSize-1) ], m_grassBaseColour1, m_grassBaseColour2) );
                    //setColourToHeight( getPlaneVertex( i, m_size-1 ), height, scaleHeight );
                }
    
                j += stride;
            }
        }

        //Set up values for next pass
        scale *= ratio;
        stride >>= 1;
    }

    if(m_size > 3)
        smoothHeightmap(m_vertices, m_kernelSize, m_size);  
}


//Averages the heightmap values
void Plane::smoothHeightmap(std::vector<Vertex*> v, int kernelSize, int edgeLength)
{
    if(kernelSize < 3)
        return;

    float smoothedHeights [edgeLength*edgeLength];  // Final smoothed values
    int kSize = kernelSize * kernelSize;            // Size of kernel matrix
    int offsetStart = (kernelSize - 1) * 0.5;       // Start position of kernel to offset count by
    int vOffset = 0;
    int hOffset = 0;
    int xIndex = 0;
    int yIndex = 0;
    int i, j, k;
    float kernelAvg;                                

    for( i = 0; i < edgeLength; i++)
    {
        for( j =0; j < edgeLength; j++)
        {
            kernelAvg = v[ get2DIndex(i,j,edgeLength) ]->y;

            for(k = 0; k < kSize; k++)
            {
                //Iterate over the convulution matrix rows and columns
                vOffset= ( ( k * kernelSize) / kSize ) - offsetStart;
                hOffset = (k % kernelSize) - offsetStart;
                kernelAvg += v[ get2DIndex(i + vOffset, j + hOffset, edgeLength) ]->y;
            }

            //Average!
            kernelAvg /= kSize;

            //Put result in a temp array so we don't smooth using pre-smoothed data
            smoothedHeights[i*edgeLength + j] = kernelAvg; 
        }
    }

    //Commit smoothed values to heightmap
    for(i = 0; i < edgeLength; i++){
        for(j = 0; j < edgeLength; j++){
            v[i*edgeLength + j]->y = smoothedHeights[i*edgeLength + j];
        }
    }
}




//Averages square centerpoint height from corners
float Plane::avgSquareVals (int i, int j, int stride)
{
    return ((float) ( getPlaneVertex( i-stride, j-stride )->y + 
            getPlaneVertex( i-stride, j+stride )->y + 
            getPlaneVertex( i+stride, j-stride )->y + 
            getPlaneVertex( i+stride, j+stride )->y ) * 0.25f );
}


//Averages diamond centerpoint values from points
float Plane::avgDiamondVals ( int i, int j, int stride, int subSize)
{
    if (i == 0){
        return ((float) ( getPlaneVertex( i, j-stride )->y + 
                         getPlaneVertex( i, j+stride )->y +
                         getPlaneVertex( i+stride, j )->y ) / 3.0f );
    }
    else if (i == subSize) {
        return ((float) ( getPlaneVertex( i, j-stride )->y + 
                         getPlaneVertex( i, j+stride )->y +
                         getPlaneVertex( i-stride, j )->y ) / 3.0f );
    }
    else if (j == 0){
        return ((float) (getPlaneVertex( i-stride, j )->y + 
                         getPlaneVertex( i+stride, j )->y +
                         getPlaneVertex( i, j+stride )->y ) / 3.0f );
    }     
    else if (j == subSize) {
        return ((float) (getPlaneVertex( i-stride, j )->y + 
                         getPlaneVertex( i+stride, j )->y +
                         getPlaneVertex( i, j-stride )->y ) / 3.0f );
    }
    else {
        return ((float) (getPlaneVertex( i-stride, j )->y + 
                         getPlaneVertex( i+stride, j )->y +
                         getPlaneVertex( i, j-stride )->y +
                         getPlaneVertex( i, j+stride )->y ) * 0.25f );
    }
}


//Returns a random float between -1.0 and 1.0
float Plane::randHeight(){
    return ((((float)rand()/(float)RAND_MAX) * 2) -1 );
}


//Sets colour of vertex based on its height 
void Plane::setColourToHeight(Vertex* vertex, float height, float scaleHeight){
    vertex->setColour( 
        (height + scaleHeight * 0.4) / ( scaleHeight*  2 ), 
        (height + scaleHeight) / ( scaleHeight*  2 ) ,
        (height + scaleHeight * 0.7) / ( scaleHeight*  2 )
    );
}
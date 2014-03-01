//
//  TerrainFace.h
//  Vcore
//
//  Created by Byron Mallett on 3/21/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef GRASSSTALK_H
#define GRASSSTALK_H 1

#include "./Mesh/Vec3.h"
#include "./Mesh/Mesh.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>

namespace Vcore {
	class GrassSegment
    {
    public:
    	GrassSegment();
    	Vec3 position;
		Vec3 direction;
        Vec3 normal;
        Vec3 colour;
		float width;
    };
    
    class GrassStalk : public Mesh
    {
    public:
        GrassStalk(
            Vec3 position, 
            int numSegments, 
            float segmentHeight, 
            float baseWidth, 
            float bendAmount, 
            Vec3 growthDirection, 
            int clock, 
            Vec3 windVector, 
            int curveInterp,
            float colourScale,
            Vec3 grassBaseColour1,
            Vec3 grassBaseColour2,
            Vec3 grassTipColour1,
            Vec3 grassTipColour2
            //float instanceHeight
        );

        enum curveType {LINEAR = 0, CUBIC = 1};
        enum componentType {WIDTH = 0, NORMALS, DIRECTION, POSITION};
        
        float getSegmentWidth(int segment)
        {
        	return m_segments[segment].width;
        }

        Vec3 getSegmentNormal(int segment)
        {
            return m_segments[segment].normal;
        }

        Vec3 getSegmentColour(int segment)
        {
            return m_segments[segment].colour;
        }

        int getCurveType(){
            return m_curveType;
        }

        int getNumCubicSegments()
        {
            int result;

            if(m_curveType == LINEAR)
                result = m_numSegments;
            else if(m_curveType == CUBIC)
                result = ((m_numSegments - 4) / 1) + 2;

            return result;
        }

        float* getCubicNormalComponents();
        float* getCubicColourComponents();




    private:

        int m_numSegments, m_curveType;

        Vec3 m_min_startColour;
        Vec3 m_max_startColour;
        Vec3 m_min_endColour;
        Vec3 m_max_endColour;
        
        std::vector<GrassSegment> m_segments;
    };


}

#endif
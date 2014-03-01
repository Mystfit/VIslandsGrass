//
//  Vertex.h
//  Vcore
//
//  Created by Byron Mallett on 21/03/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#ifndef VERTEX_H
#define VERTEX_H 1

#include "Vec3.h"

namespace Vcore
{
    class Vertex : public Vec3
    {
    public:
        Vertex();
        Vertex( float x_pos, float y_pos, float z_pos );

        enum ColourComponents { COL_R = 0, COL_G, COL_B, COL_A };

        
        Vec3 getNormal()
        {
            return m_normal;
        }
        
        Vec3 setNormal(Vec3 w)
        {
            m_normal = w;
        }

        void addFaceNormal(Vec3 faceNormal);

        void setColour(float r, float g, float b)
        {
            m_colour.x = r;
            m_colour.y = g;
            m_colour.z = b;
        }

        void setColour(float r, float g, float b, float a)
        {
            setColour(r,g,b);
            m_alpha = a;
        }

        void setVecColour(Vec3 colour)
        {
            setColour(colour.x,colour.y,colour.z);
        }

        float getColour(int colourType)
        {
            if(colourType == COL_R)
                return m_colour.x;
            else if(colourType == COL_G) 
                return m_colour.y;
            else if(colourType == COL_B) 
                return m_colour.z;
            else if(colourType == COL_A)
                return m_alpha;
            else 
                return 0.0f;
        }

        void u(float value)
        {
            uCoord = uCoord;
        }

        float u(){
            return uCoord;
        }

        void v(float value)
        {
            vCoord = vCoord;
        }

        float v(){
            return vCoord;
        }
        
    private:
        float uCoord;
        float vCoord;
        Vec3 m_normal;
        Vec3 m_colour;
        float m_alpha;
    };
}

#endif
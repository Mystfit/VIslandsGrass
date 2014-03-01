//
//  Vec3.h
//  Vcore
//
//  Created by Byron Mallett on 3/14/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef VEC3_H
#define VEC3_H 1

#define _USE_MATH_DEFINES
#include <math.h>

namespace Vcore
{
    //Point class
    //------------------------------
    class Vec3 
    {
    public:
        float x,y,z;

        Vec3();
        Vec3( float x_pos, float y_pos, float z_pos );


        //--------------------------------
        // Tools
        //--------------------------------
        static float lerp(float t, float a, float b);
        static Vec3 lerpColour(float t, Vec3 a, Vec3 b );

        //Operator overloads
        Vec3 operator+ ( const Vec3 &component ) const;
        Vec3 operator- ( const Vec3 &component ) const;
        Vec3 operator* ( const Vec3 &component ) const;

        float length();
        float horizAngle();
        float vertAngle();
        static float degToRad(float value);
        static float radToDeg(float value);

        static Vec3 cross( const Vec3 &u, const Vec3 &v );
        static Vec3 normal(Vec3 a, Vec3 b);
        static Vec3 normal(Vec3 a, Vec3 b, Vec3 origin);

        Vec3 normalize();
        Vec3 normalize( float length );
    };
}

#endif

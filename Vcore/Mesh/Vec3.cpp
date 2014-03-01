//
//  Point.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Vec3.h"
#include <iostream>


using namespace Vcore;

Vec3::Vec3(){
    x = y = z = 0.0f;
}

Vec3::Vec3(float x_pos, float y_pos, float z_pos) : x(x_pos), y(y_pos), z(z_pos)
{
}

float Vec3::lerp(float t, float a, float b){
    return  a + t * (b - a);
} 

Vec3 Vec3::lerpColour(float t, Vec3 a, Vec3 b )
{
    return Vec3(lerp(t, a.x, b.x), lerp(t, a.y, b.y), lerp(t, a.z, b.z));
}


//Vector operator overloads
//-----------------------------------
Vec3 Vec3::operator+ (const Vec3 &component) const{  
    Vec3 temp;
    temp.x = this->x +  component.x;
    temp.y = this->y +  component.y;
    temp.z = this->z +  component.z;

    return (temp);
}

Vec3 Vec3::operator- (const Vec3 &component) const{  
    Vec3 temp;
    temp.x = this->x -  component.x;
    temp.y = this->y -  component.y;
    temp.z = this->z -  component.z;
    
    return (temp);
}

Vec3 Vec3::operator* (const Vec3 &component) const{  
    Vec3 temp;
    temp.x = this->x *  component.x;
    temp.y = this->y *  component.y;
    temp.z = this->z *  component.z;
    
    return (temp);
}


//-----------------------------------
//Vector math
float Vec3::length()
{
    return sqrt ( x*x + y*y + z*z);  
}

float Vec3::vertAngle()
{
    float result;
    if(x == 0.0f && y == 0.0f){
        result = 0.0f;
    }else{
        result = atan2(y,x);
    }

    return result;
}

float Vec3::horizAngle()
{
    float result;
    if(x == 0.0f && z == 0.0f){
        result = 0.0f;
    }else{
        result = atan2(z,x);
    }

    return result;
}

float Vec3::degToRad(float value)
{
    return value * (M_PI / 180);
}

float Vec3::radToDeg(float value)
{
    return value * (180 / M_PI);
}


Vec3 Vec3::cross(const Vec3 &u, const Vec3 &v)
{
    return Vec3 ( (u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x) );
}

Vec3 Vec3::normal(Vec3 a, Vec3 b)
{
    return normal(a, b, Vec3());
}

Vec3 Vec3::normal(Vec3 a, Vec3 b, Vec3 origin = Vec3())
{
    return Vec3::cross(  a - origin, b - origin ).normalize();
}


Vec3 Vec3::normalize()
{
    return normalize( 1.0f );
}

Vec3 Vec3::normalize( float newLength )
{

    if(newLength == 0)
        return *this;

    float l = length();

    this->x /= l;
    this->y /= l;
    this->z /= l;

    this->x *= newLength;
    this->y *= newLength;
    this->z *= newLength;

    return *this;
}


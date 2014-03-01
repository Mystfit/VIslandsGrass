#include "PerlinNoise.h"

#include <math.h>

using namespace core;

PerlinNoise::PerlinNoise(int i_seed, float i_persist, float i_freq, float i_amp, int i_octaves)
    :
    m_seed(i_seed),
    m_persistence(i_persist),
    m_frequency(i_freq),
    m_amplitude(i_amp),
    m_octaves(i_octaves)
{
}   

float PerlinNoise::noise2d(float x, float y)
{
    register int n = int(x + y * 57);
    n = (n << 13) ^ n;
    register int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0 - float(t) * 0.931322574615478515625e-9;
}

float PerlinNoise::noise3d(float x, float y, float z)
{
    register int n = x + y * 57 + z * 3366;
    n = (n << 13) ^ n;
    register float t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0 - t * 0.931322574615478515625e-9;
}

float PerlinNoise::interpolate(float x, float y, float a)
{
    register float f = (1 - cos(a * 3.1415927)) * 0.5;
    return x * (1-f) + y * f;
}

float PerlinNoise::smoothedNoise2d(float x, float y)
{
    // float value = 0;
    // for(register int i=-1; i<=1; ++i)
    // {
    //     for(register int j=-1; j<=1; ++j)
    //     {
    //         value += noise2d(x-i,y-j);
    //     }
    // }
    // return value/3.0;

    float corners = ( noise2d(x-1, y-1) + noise2d(x+1, y-1) + noise2d(x-1, y+1) + noise2d(x+1, y+1) ) * 0.0625;
    float sides = ( noise2d(x-1, y) + noise2d(x+1, y) + noise2d(x, y-1) + noise2d(x, y+1) ) *  0.125;
    float center = noise2d(x, y) * 0.25;
    return corners + sides + center;
}

float PerlinNoise::smoothedNoise3d(float x, float y, float z)
{
    // register float value = 0;
    // for(register int i=-1; i<=1; ++i)
    // {
    //     for(register int j=-1; j<=1; ++j)
    //     {
    //         for(register int k=-1; k<=1; ++k)
    //         {
    //             value += noise3d(x-i, y-j, z-k);
    //         }
    //     }
    // }
    // return value/27;

    float corners = (noise3d(x-1, y-1, z-1) + noise3d(x+1, y-1, z-1) + noise3d(x-1, y+1, z-1) + noise3d(x+1, y+1, z-1) + noise3d(x-1, y-1, z+1) + noise3d(x+1, y-1, z+1) + noise3d(x-1, y+1, z+1) + noise3d(x+1, y+1, z+1)) * 0.03125;
    float sides = (noise3d(x-1, y, z-1) + noise3d(x+1, y, z-1) + noise3d(x, y-1, z-1) + noise3d(x, y+1, z-1) + noise3d(x-1, y, z+1) + noise3d(x+1, y, z+1) + noise3d(x, y-1, z+1) + noise3d(x, y+1, z+1)) *  0.0625;
    float center = noise3d(x, y, z) * 0.125;
    return corners + sides + center;
}

float PerlinNoise::compute2d(float x, float y)
{
    //** Floor the coords
    register int intX = (int)x;
    register int intY = (int)y;

    //** Find the offsets from the coords
    float fractX = x - intX;
    float fractY = y - intY;

    float v1 = smoothedNoise2d(intX, intY);
    float v2 = smoothedNoise2d(intX + 1, intY);
    float v3 = smoothedNoise2d(intX, intY + 1);
    float v4 = smoothedNoise2d(intX + 1, intY + 1);

    float i1 = interpolate(v1 , v2 , fractX);
    float i2 = interpolate(v3 , v4 , fractX);

    float fvalue = interpolate(i1 , i2 , fractY);
    return fvalue;
}

float PerlinNoise::perlinNoise2d(float x, float y)
{
    float total = 0;
    float freq = m_frequency;
    float amp = m_amplitude;
    float persist = m_persistence;

    for(register int i=0; i< m_octaves; ++i)
    {
        total += (compute2d( x * freq + m_seed, y * freq + m_seed ) * amp);
        freq *= 2;
        amp = amp - (amp * persist);
        persist *= m_persistence;
    }
    return total;
}

float PerlinNoise::compute3d(float x, float y, float z) 
{
    //** Floor the coords
    register int intX = (int)x;
    register int intY = (int)y;
    register int intZ = (int)z;

    //** Find the offsets from the coords
    float fractX = x - intX;
    float fractY = y - intY;
    float fractZ = z - intZ;

    float v1 = smoothedNoise3d(intX, intY, intZ);
    float v2 = smoothedNoise3d(intX + 1, intY, intZ);
    float v3 = smoothedNoise3d(intX, intY + 1, intZ);
    float v4 = smoothedNoise3d(intX + 1, intY + 1, intZ);

    float i1 = interpolate(v1 , v2 , fractX);
    float i2 = interpolate(v3 , v4 , fractX);

    float fvalue1 = interpolate(i1 , i2 , fractY);

    float v5 = smoothedNoise3d(intX, intY, intZ + 1);
    float v6 = smoothedNoise3d(intX + 1, intY, intZ + 1);
    float v7 = smoothedNoise3d(intX, intY + 1, intZ + 1);
    float v8 = smoothedNoise3d(intX + 1, intY + 1, intZ + 1);

    float i3 = interpolate(v5 , v6 , fractX);
    float i4 = interpolate(v7 , v8 , fractX);

    float fvalue2 = interpolate(i3 , i4 , fractY);

    return interpolate(fvalue1 , fvalue2 , fractZ);
}

float PerlinNoise::perlinNoise3d (float x, float y, float z)
{
    float total = 0;
    float freq = m_frequency;
    float amp = m_amplitude;
    float persist = m_persistence;

    for(register int i=0; i< m_octaves; ++i)
    {
        total += (compute3d( x * freq + m_seed, y * freq + m_seed, z * freq + m_seed ) * amp);
        freq *= 2;
        amp = amp - (amp * persist);
        persist *= m_persistence;
    }
    return total;
}
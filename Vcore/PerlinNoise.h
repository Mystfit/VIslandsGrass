#ifndef PERLINNOISE_H
#define PERLINNOISE_H

namespace core
{
    class PerlinNoise
    {
        private:
            int m_seed;
            float m_persistence;
            float m_frequency;
            float m_amplitude;
            int m_octaves;

            //** Cosine interpolation
            float interpolate(float x, float y, float a);
            float smoothedNoise2d(float x, float y);
            float smoothedNoise3d(float x, float y, float z);

            //** Compute the raw value of the perlin noise for offsetting against the amplitude
            float compute2d(float x, float y);
            float compute3d(float x, float y, float z);

        public:
            PerlinNoise(int i_seed=0, float i_persist=0, float i_freq=0, float i_amp=0, int i_octaves=0);

            float noise2d(float x, float y);
            float noise3d(float x, float y, float z);
            float perlinNoise2d(float x, float y);
            float perlinNoise3d(float x, float y, float z);

            //** Accessors
            int seed() const { return m_seed; }
            int& seed() { return m_seed; }

            float persistence() const { return m_persistence; }
            float& persistence() { return m_persistence; }

            float frequency() const { return m_frequency; }
            float& frequency() { return m_frequency; }

            float amplitude() const { return m_amplitude; }
            float& amplitude() { return m_amplitude; }

            int octaves() const { return m_octaves; }
            int& octaves() { return m_octaves; }

    };
}


#endif
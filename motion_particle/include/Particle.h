#ifndef PARTICLE_H
#define PARTICLE_H
#include <raylib.h>
#include <iostream>
#include <cmath>
#define EDGE 800
#define G 9.80665
#define G1 6.67408e-11
#define M 100
#define KM 10000000
#define moon 7.3476e22
#define earth 5.97219e24

using namespace std;


class Particle
{
    public:
        Particle();
        virtual ~Particle();
        void Draw();
        void Attract(Vector2);
        Vector2 GetNormal(Vector2);
        double GetDistance(Vector2);
        void Move();
        void AttractMoon();
        void AttractMouse();

    protected:

    private:
        Vector2 position;
        Vector2 velocity;
        Vector2 acceleration;
        double oldPosToEarth;
        double oldPosToMoon;
        Color color;
};

#endif // PARTICLE_H

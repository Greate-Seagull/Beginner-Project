#include "Particle.h"

Particle::Particle()
{
    position.x = GetRandomValue(0, EDGE - 1);
    position.y = GetRandomValue(0, EDGE - 1);

    velocity.x = (float) GetRandomValue(-100, 100) / 100;
    velocity.y = (float) GetRandomValue(-100, 100) / 100;

    acceleration.x = 0;
    acceleration.y = 0;

    oldPosToMoon = GetDistance({600, 800});
    oldPosToEarth = GetDistance({200, 800});

    color = {(unsigned char) GetRandomValue(0, 255),
             (unsigned char) GetRandomValue(0, 255),
             (unsigned char) GetRandomValue(0, 255),
             (unsigned char) GetRandomValue(0, 255),};
}

Particle::~Particle()
{
    //dtor
}

void Particle::Draw(){
    DrawPixel(position.x, position.y, color);
}

double Particle::GetDistance(Vector2 posToGet){
    float x = posToGet.x - position.x;
    float y = posToGet.y - position.y;
    float dis = sqrt(x*x + y*y);
    if(dis < 1) dis = 1;
    return dis;
}

Vector2 Particle::GetNormal(Vector2 posToGet){
    Vector2 direction;
    direction.x = posToGet.x - position.x;
    direction.y = posToGet.y - position.y;

    double absolute_dis = GetDistance(posToGet);

    direction.x /= absolute_dis;
    direction.y /= absolute_dis;

    return direction;
}

void Particle::Attract(Vector2 posToAttract){
    //double distance = GetDistance(posToAttract);

    Vector2 direction = GetNormal(posToAttract);

    acceleration.x = G * direction.x;
    acceleration.y = G * direction.y;
}

void Particle::Move(){
    position.x += velocity.x / 60 + acceleration.x / 60 / 60 / 2 * M;
    position.y += velocity.y / 60 + acceleration.y / 60 / 60 / 2 * M;

    velocity.x += acceleration.x / 60;
    velocity.y += acceleration.y / 60;

    if(position.x < 0)
        position.x += EDGE;
    else if(position.x >= EDGE)
        position.x -= EDGE;
    if(position.y < 0)
        position.y += EDGE;
    else if(position.y >= EDGE)
        position.y -= EDGE;
}

void Particle::AttractMoon(){
    double virtual_dis = GetDistance(Vector2{400, 400});
    double distance = 384.400 * KM / 100 * virtual_dis / oldPosToMoon;
    Vector2 direction = GetNormal(Vector2{400, 400});

    acceleration.x = direction.x * moon * G1 * M / (distance * distance);
    acceleration.y = direction.y * moon * G1 * M / (distance * distance);
}

void Particle::AttractMouse(){
    Vector2 mouse = GetMousePosition();
    double virtual_dis = GetDistance(mouse);
    double distance = 6.371 * KM * virtual_dis / oldPosToEarth;
    Vector2 direction = GetNormal(mouse);

    acceleration.x = direction.x * earth * G1 / (distance * distance);
    acceleration.y = direction.y * earth * G1 / (distance * distance);
}

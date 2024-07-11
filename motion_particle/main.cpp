#include <iostream>
#include <Particle.h>

using namespace std;

int main()
{
    InitWindow(800, 800, "Particle");
    SetTargetFPS(60);

    int particleCount = 200000;
    Particle* box = new Particle[particleCount];

    while(WindowShouldClose() == false){
        BeginDrawing();

        ClearBackground(BLACK);
        for(int i = 0; i < particleCount; i++){
            box[i].Draw();

            box[i].Attract(GetMousePosition());

            box[i].Move();

            box[i].AttractMoon();

            box[i].Move();

//            box[i].AttractMouse();
//
//            box[i].Move();
        }

        EndDrawing();
    }

    delete[] box;

    return 0;
}

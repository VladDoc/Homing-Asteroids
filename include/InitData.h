
#ifndef INITDATA_H
#define INITDATA_H

#include "Vector2d.h"

struct InitData
{
    Vector2D<int> windowSize;
    Vector2D<int> mapSize;

    int num_enemies;
    int num_ammo;

    InitData(int argc, char** argv);
};

#endif // INITDATA_H

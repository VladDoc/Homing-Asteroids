#ifndef MAP_H
#define MAP_H

#include <vector>

#include "UpdateData.h"
#include "Asteroid.h"
#include "Vector2d.h"
#include "Bullet.h"
#include "Player.h"

class Map
{
    public:
        Map(int asteroids_qty, int max_bullets,
            const UpdateData& data);

        virtual ~Map();
        virtual void update(const UpdateData& data);
        virtual void draw(const UpdateData& data);
    protected:
        std::vector<Asteroid> asteroids;
        Vector2D<int> mapSize;
        std::vector<Bullet> bullets;

        size_t remainingAsteroids;
        size_t remainingBullets;
    private:
};

#endif // MAP_H

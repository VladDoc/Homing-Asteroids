#include <cassert>

#include "Bullet.h"
#include "Constants.h"
#include "Player.h"
#include "util.h"


static const char* path = "data/bullet.png";

Bullet::Bullet() : GameObject(path)
{
    this->collisionRadius *= 4;
}

Bullet::~Bullet() = default;

void Bullet::update(const UpdateData& data)
{
    using namespace Constants;

    if(this->empty()) return;

    Vector2D<float> motionDelta;
    Vector2D<float> playersPos;

    data.player->getMiddlePoint(playersPos.x, playersPos.y);

    motionDelta = playersPos;
    motionDelta -= data.lastPlayerPos;

    position = position - motionDelta;


    //velocity.x += accel.x * data.frametime / mass;
    //velocity.y += accel.y * data.frametime / mass;

    //position += velocity;

    position.x += util::interpolateValue(data.frametime, velocity.x);
    position.y += util::interpolateValue(data.frametime, velocity.y);

    //assert(!util::offMap(position, data.mapBoundaries));

    //position.x = util::clampLooping(position.x, 0.0f, data.mapBoundaries.x);
    //position.y = util::clampLooping(position.y, 0.0f, data.mapBoundaries.y);

    //position.x = util::clamp(position.x, 0.0f, (float)data.mapBoundaries.x - this->sprite_.w);
    //position.y = util::clamp(position.y, 0.0f, (float)data.mapBoundaries.y - this->sprite_.h);

    sprite_.x = (int)position.x;
    sprite_.y = (int)position.y;
}

void Bullet::unUpdate(const UpdateData& data)
{
}

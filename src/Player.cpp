#include <string>
#include <cmath>
#include <limits>
#include <cassert>

#include "Player.h"
#include "Constants.h"
#include "util.h"

static const char* path = "data/spaceship.png";

Player::Player() : GameObject(path)
{
    getScreenSize(sprite_.x, sprite_.y);

    sprite_.x -= sprite_.w;
    sprite_.y -= sprite_.h;

    sprite_.x /= 2;
    sprite_.y /= 2;

    position.x = sprite_.x;
    position.y = sprite_.y;
}

Player::Player(float x, float y, float angle) : GameObject(path)
{
    sprite_angle = util::clampLooping(angle, 0.0f, 2 * Constants::pi);

    position.x = x;
    position.y = y;

    sprite_.x = x;
    sprite_.y = y;
}

Player::~Player()
{

}

//void Player::draw() const
//{
//    drawSpriteR((Sprite *) sprite_.getSprite(),
//                 position.x, position.y, this->sprite_angle);
//}
//
//void Player::draw(const UpdateData& data) const
//{
//    if(sprite_.empty()) return;
//
//    //this->draw();
//
//    Vector2D<float> screenCoords{(float)data.screenSize.x / 2,
//                                 (float)data.screenSize.y / 2};
//
//    Vector2D<float> camera = util::getCamera(*data.player,
//                                              data.screenSize,
//                                              data.mapBoundaries);
//
//    util::mapToScreenSpaceAffectedByCamera(screenCoords, data.screenSize,
//                                           this->position, camera);
//
//
////    assert(screenCoords.x >= 0);
////    assert(screenCoords.y >= 0);
////
////    assert(screenCoords.x < data.screenSize.x);
////    assert(screenCoords.y < data.screenSize.y);
//
//    drawSpriteR((Sprite *) sprite_.getSprite(),
//                screenCoords.x, screenCoords.y, sprite_angle);
//
//}

static Vector2D<int> lastMousePos;

void Player::update(const UpdateData& data)
{
    using namespace Constants;

    if(this->empty()) return;

    Vector2D<float> onScreen = position;

    Vector2D<float> camera = util::getCamera(*this,
                                             data.screenSize,
                                             data.mapBoundaries);

    util::mapToScreenSpaceAffectedByCamera(onScreen, data.screenSize,
                                           this->position, camera);

    if(lastMousePos != data.mouseCoords) {
        this->sprite_angle = util::angleBetweenPoints(data.mouseCoords,
                                                           onScreen);
    }

    if(data.controls.turnRight) {
        sprite_angle += util::interpolateValue(data.frametime,
                                               util::degToRad(45.0f));
    }
    if(data.controls.turnLeft)  {
        sprite_angle -= util::interpolateValue(data.frametime,
                                               util::degToRad(45.0f));
    }

    //sprite_angle = util::clampLooping(sprite_angle, 0.0f, 2 * pi);

    if(data.controls.goForward)    {
        this->position.x += util::interpolateValue(data.frametime,
                                          sinf(sprite_angle) * this->maxSpeed);
        this->position.y += util::interpolateValue(data.frametime,
                                         -cosf(sprite_angle) * this->maxSpeed);
    }
    if(data.controls.goBackward)  {
        this->position.x += util::interpolateValue(data.frametime,
                                         -sinf(sprite_angle) * this->maxSpeed);
        this->position.y += util::interpolateValue(data.frametime,
                                          cosf(sprite_angle) * this->maxSpeed);
    }

    if(data.controls.strafeLeft)    {
        this->position.x += util::interpolateValue(data.frametime,
                                                   -sinf(sprite_angle + pi / 2) * this->maxSpeed);
        this->position.y += util::interpolateValue(data.frametime,
                                                   cosf(sprite_angle + pi / 2) * this->maxSpeed);
    }
    if(data.controls.strafeRight)  {
        this->position.x += util::interpolateValue(data.frametime,
                                                   sinf(sprite_angle + pi / 2) * this->maxSpeed);
        this->position.y += util::interpolateValue(data.frametime,
                                                   -cosf(sprite_angle + pi / 2) * this->maxSpeed);
    }

    lastMousePos = data.mouseCoords;

    position.x = util::clamp(position.x, 0.0f, (float)data.mapBoundaries.x - this->sprite_.w);
    position.y = util::clamp(position.y, 0.0f, (float)data.mapBoundaries.y - this->sprite_.h);
}

void Player::unUpdate(const UpdateData& data)
{

}



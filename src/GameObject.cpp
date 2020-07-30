#include <type_traits>

#include <cassert>
#include "GameObject.h"
#include "Framework.h"
#include "util.h"
#include "Constants.h"
#include "Player.h"


GameObject::GameObject()
{
    //ctor
}


GameObject::GameObject(const char* sprite_path) : sprite_(sprite_path)
{
    this->collisionRadius = std::hypot(sprite_.w / 2, sprite_.h / 2) * 0.75f;
}


GameObject::GameObject(const U_Sprite& ref) : sprite_(ref)
{
    this->collisionRadius = std::hypot(sprite_.w / 2, sprite_.h / 2) * 0.75f;
}


GameObject::~GameObject()
{

}


bool GameObject::checkCollision(const GameObject& another) const
{
    float x1, y1;
    float x2, y2;

    this->getMiddlePoint(x1, y1);
    another.getMiddlePoint(x2, y2);

    float dx = x1 - x2;
    float dy = y1 - y2;

    float distance = std::hypot(dx, dy);

    if(distance < this->collisionRadius + another.collisionRadius) {
        return true;
    }

    return false;
}


void GameObject::draw() const
{
    if(sprite_.empty()) return;
    drawSpriteR((Sprite *) sprite_.getSprite(),
                this->position.x, this->position.y, this->sprite_angle);
}

void GameObject::draw(const UpdateData& data) const
{
    if(sprite_.empty()) return;

   // this->draw();

    Vector2D<float> screenCoords;

    Vector2D<float> camera = util::getCamera(*data.player,
                                              data.screenSize,
                                              data.mapBoundaries);

    util::mapToScreenSpaceAffectedByCamera(screenCoords, data.screenSize,
                                           this->position, camera);

    drawSpriteR((Sprite *) sprite_.getSprite(),
                (int)screenCoords.x, (int)screenCoords.y, sprite_angle);


}

bool GameObject::empty() const
{
    return sprite_.empty();
}

void GameObject::setEmpty(bool flag)
{
    sprite_.setEmpty(flag);
}

const U_Sprite& GameObject::getU_Sprite() const
{
    return this->sprite_;
}

void GameObject::setCollisionRadius(float value)
{
    this->collisionRadius = value;
}

void GameObject::getCollisionRadius(float& value) const
{
    value = this->collisionRadius;
}

void GameObject::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;

    sprite_.x = (int)x;
    sprite_.y = (int)y;

}

void GameObject::getPosition(float& x, float& y) const
{
    x = position.x;
    y = position.y;
}

void GameObject::setMiddlePoint(float x, float y)
{
    position.x = x - sprite_.w / 2;
    position.y = y - sprite_.h / 2;

    sprite_.x = (int)position.x;
    sprite_.y = (int)position.y;
}

void GameObject::getMiddlePoint(float& x, float& y) const
{
    x = position.x + sprite_.w / 2;
    y = position.y + sprite_.h / 2;
}

void GameObject::setSprite(const U_Sprite& sprite)
{
    this->sprite_ = sprite;
}

void GameObject::setSprite(const char* path)
{
    U_Sprite sprite(path);
    this->sprite_ = sprite;
}


void GameObject::setMovement(const Vector2D<float>& speed,
                             const Vector2D<float>& acceleration)
{
    this->velocity = speed;
    this->accel = acceleration;
}

void GameObject::getMovement(Vector2D<float>& speed,
                             Vector2D<float>& acceleration) const
{
    speed = this->velocity;
    acceleration = this->accel;
}

void GameObject::setRotation(float speed,
                             float acceleration,
                             float start_angle)
{
    this->angular_velocity = speed;
    this->angular_accel = acceleration;
    this->sprite_angle = start_angle;
}

void GameObject::getRotation(float& angle,
                             float& speed,
                             float& acceleration) const
{
    speed = this->angular_velocity;
    acceleration = this->angular_accel;
    angle = this->sprite_angle;
}

void GameObject::setMass(float mass)
{
    this->mass = mass;
}

void GameObject::getMass(float& m) const
{
    m = this->mass;
}


#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <random>
#include <cassert>

#include "Asteroid.h"
#include "Constants.h"
#include "util.h"
#include "Player.h"

static const std::map<const AsteroidType, std::string> asteroidSpritePaths =
{
        {AsteroidType::BIG, "data/big_asteroid.png"},
        {AsteroidType::SMALL, "data/small_asteroid.png"}
};

static const float masses[(int)AsteroidType::COUNT] = {10.0f, 30.0f};

Asteroid::Asteroid()
{

}

Asteroid::Asteroid(const char* path) :
    GameObject(path)
{
    if(path == asteroidSpritePaths.at(AsteroidType::SMALL)) {
        this->type = AsteroidType::SMALL;
    } else {
        this->type = AsteroidType::BIG;
    }
    this->mass = masses[(uint8_t)type];
}

Asteroid::Asteroid(const AsteroidType type) :
    GameObject(asteroidSpritePaths.at(type).c_str())
{
    this->type = type;
    this->mass = masses[(uint8_t)type];
}

Asteroid::Asteroid(const U_Sprite& sprite, const AsteroidType type) :
    GameObject(sprite)
{
    this->type = type;
    this->mass = masses[(uint8_t)type];
}

Asteroid::~Asteroid()
{

}

void Asteroid::update(const UpdateData& data)
{
    using namespace Constants;

    if(this->empty()) return;

    Vector2D<float> motionDelta;
    Vector2D<float> playersPos;
    data.player->getMiddlePoint(playersPos.x, playersPos.y);

    motionDelta = playersPos;
    motionDelta -= data.lastPlayerPos;

    position -= motionDelta;

    velocity.x += accel.x * data.frametime / mass;
    velocity.y += accel.y * data.frametime / mass;

    position.x += util::interpolateValue(data.frametime, velocity.x);
    position.y += util::interpolateValue(data.frametime, velocity.y);

    Vector2D<float> toPlayer = playersPos - position;
    toPlayer.Normalize();

    position.x += util::interpolateValue(data.frametime, toPlayer.x * speedTowardsPlayer);
    position.y += util::interpolateValue(data.frametime, toPlayer.y * speedTowardsPlayer);

    //position.x = util::clampLooping(position.x, 0.0f, data.mapBoundaries.x);
    //position.y = util::clampLooping(position.y, 0.0f, data.mapBoundaries.y);

    //position.x = util::clamp(position.x, 0.0f, (float)data.mapBoundaries.x - this->sprite_.w);
    //position.y = util::clamp(position.y, 0.0f, (float)data.mapBoundaries.y - this->sprite_.h);

    angular_velocity += angular_accel * data.frametime / mass;

    this->sprite_angle = util::vectorAngle(toPlayer.x, toPlayer.y);

//    sprite_.x -= motion.x;
//    sprite_.y -= motion.y;
}

void Asteroid::unUpdate(const UpdateData& data)
{

}


std::vector<Asteroid> Asteroid::breakInPieces(const GameObject& obj)
{
    this->setEmpty(true);
    if(this->type == AsteroidType::SMALL) {
        return std::vector<Asteroid>();
    } else {
        std::vector<Asteroid> asteroids;

        asteroids.push_back(Asteroid(AsteroidType::SMALL));
        asteroids.push_back(Asteroid(AsteroidType::SMALL));

        asteroids[0].setPosition(this->position.x, this->position.y);
        asteroids[1].setPosition(this->position.x, this->position.y);

        asteroids[0].setEmpty(false);
        asteroids[1].setEmpty(false);


        std::uniform_real_distribution<float> distangle(0.0f,
                                                     Constants::pi * 2);
        std::uniform_real_distribution<float> distrotation(0.0f, 0.001f);
        std::uniform_real_distribution<float> distspeed(0.0f, Asteroid::maxSpeed);

        std::mt19937 random;
        random.seed(time(NULL));
        float angle = distangle(random);

        float speed = distspeed(random);

        Vector2D<float> obj_vel;
        Vector2D<float> obj_acc;

        obj.getMovement(obj_vel, obj_acc);

        float obj_angle = util::vectorAngle(obj_vel.x, obj_vel.y);

        asteroids[0].setMovement(
                util::lengthWithAngleToVector(obj_angle - angle, speed), {0,0});
        asteroids[1].setMovement(
                util::lengthWithAngleToVector(obj_angle + angle, speed), {0,0});

        asteroids[0].setRotation(distrotation(random), 0.0f, distangle(random));
        asteroids[1].setRotation(distrotation(random), 0.0f, distangle(random));

        return asteroids;
    }
}

bool operator<(const Asteroid& a, const Asteroid& b)
{
    Vector2D<float> a_pos;
    Vector2D<float> b_pos;

    a.getPosition(a_pos.x, a_pos.y);
    b.getPosition(b_pos.x, b_pos.y);

    return !a.empty() && a_pos < b_pos;
}

void Asteroid::collide(Asteroid& a)
{
    Vector2D<float> vel1;
    Vector2D<float> vel2;

    vel1.x = (this->velocity.x * (this->mass - a.mass) +
             (2 * a.mass * a.velocity.x)) / (this->mass + a.mass);
    vel1.y = (this->velocity.y * (this->mass - a.mass) +
             (2 * a.mass * a.velocity.y)) / (this->mass + a.mass);
    vel2.x = (a.velocity.x * (a.mass - this->mass) +
             (2 * this->mass * this->velocity.x)) / (this->mass + a.mass);
    vel2.y = (a.velocity.y * (a.mass - this->mass) +
             (2 * this->mass * this->velocity.y)) / (this->mass + a.mass);

    Vector2D<float> pos1;
    Vector2D<float> pos2;

    this->getMiddlePoint(pos1.x, pos1.y);
    a.getMiddlePoint(pos2.x, pos2.y);

    this->setMovement(vel1, {0,0});
    a.setMovement(vel2, {0, 0});

    pos1 += vel1;
    pos2 += vel2;

    this->setMiddlePoint(pos1.x, pos1.y);
    a.setMiddlePoint(pos2.x, pos2.y);
}

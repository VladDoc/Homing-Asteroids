#include "Map.h"

#include <ctime>
#include <random>
#include <algorithm>

#include "Vector2d.h"
#include "util.h"


Map::Map(int asteroids_qty, int max_bullets,
         const UpdateData& data)
{
    Player& player = *data.player;
    this->mapSize = data.mapBoundaries;

    this->bullets.resize(max_bullets, Bullet());

    this->asteroids.reserve(asteroids_qty);

    for(auto& a : bullets) {
        a.setEmpty(true);
    }

    float playerWidth;
    player.getCollisionRadius(playerWidth);
    playerWidth *= 5;

    Vector2D<float> playerPos;
    player.getMiddlePoint(playerPos.x, playerPos.y);

    std::uniform_int_distribution<int> distint(1, 3);
    std::uniform_int_distribution<int> distx(0, data.mapBoundaries.x);
    std::uniform_int_distribution<int> disty(0, data.mapBoundaries.y);

    std::uniform_real_distribution<float> distfloat(-Asteroid::maxSpeed,
                                                     Asteroid::maxSpeed);

    std::uniform_real_distribution<float> distanglespeed(0.0f, 0.005f);
    std::uniform_real_distribution<float> distangle(0.0f, 2 * Constants::pi);

    std::mt19937 random;

    random.seed(time(NULL));

    U_Sprite small("data/small_asteroid.png");
    U_Sprite big("data/big_asteroid.png");
    for(int i = 0; i < asteroids_qty;  ++i) {
        Vector2D<int> mapIndeces;

        mapIndeces.x = distx(random);
        mapIndeces.y = disty(random);

        if(util::withinArea(mapIndeces, (Vector2D<int>)playerPos, playerWidth)) {
            asteroids_qty += 1;
            continue;
        }

        if(distint(random) == 1) {
            asteroids.push_back(Asteroid(big, AsteroidType::BIG));
            asteroids.back().setRotation(distanglespeed(random) / 3, 0.0f, distangle(random));
        } else {
            asteroids.push_back(Asteroid(small, AsteroidType::SMALL));
            asteroids.back().setRotation(distanglespeed(random), 0.0f, distangle(random));
        }

        asteroids.back().setMiddlePoint(mapIndeces.x, mapIndeces.y);
        asteroids.back().setMovement({distfloat(random), distfloat(random)},
                                     {0,0});
    }
    std::sort(asteroids.begin(), asteroids.end());
    remainingAsteroids = asteroids.size();
    remainingBullets = max_bullets;
}

Map::~Map()
{
    //dtor
}

void Map::update(const UpdateData& data)
{

    if(remainingAsteroids == 0) {
        data.gameOver = true;
        return;
    }

    //std::sort(asteroids.begin(), asteroids.end());

    for(auto& b : bullets) {
        b.update(data);

        Vector2D<float> b_pos;
        b.getPosition(b_pos.x, b_pos.y);

        if(util::offMap(b_pos, data.mapBoundaries)) {
            b.setEmpty(true);
            remainingBullets += 1;
        }
    }

    if(data.controls.fire) {

        size_t index = 0;
        for(; index <= bullets.size(); ++index) {
            if(index == bullets.size() ||
               bullets[index].empty()) break;
        }

        if(index == bullets.size() || remainingBullets == 0) {

        } else {

            bullets[index].setEmpty(false);

            Vector2D<float> pos;
            Vector2D<float> speed;
            Vector2D<float> whatever; // placeholder
            float angle;

            data.player->getMiddlePoint(pos.x, pos.y);

            data.player->getRotation(angle, whatever.x, whatever.y);

            angle = util::clampLooping(angle, 0.0f, 2 * Constants::pi);

            data.player->getMovement(speed, whatever);

            speed.x += sinf(angle) * Bullet::defSpeed;
            speed.y -= cosf(angle) * Bullet::defSpeed;

            bullets[index].setMiddlePoint(pos.x, pos.y);

            bullets[index].setMovement(speed, {0.0f, 0.0f});

            data.controls.fire = false;
            remainingBullets -= 1;
        }
    }

    std::vector<Asteroid> temp;
    temp.reserve(2);

    // Pushing into main vector invalidates iterators
    // To keep being UB free you've got to copy it over instead
    std::vector<Asteroid> temp2; // YaY unique and well thought out names

    for(auto& a : asteroids) {
        if(a.empty()) continue;
        a.update(data);

        for(auto& b : bullets) {
            if(b.empty()) continue;
            if(a.checkCollision(b)) {
                b.setEmpty(true);
                remainingAsteroids -= 1;
                remainingBullets += 1;

                temp = a.breakInPieces(b);

                for(auto& aa : temp) {
                    temp2.push_back(aa);
                }
            }
        }

        Vector2D<float> pos;
        float radius;

        a.getPosition(pos.x, pos.y);
        a.getCollisionRadius(radius);

        Asteroid lower;

        lower.setPosition(pos.x - radius,
                          pos.y - radius);

        Asteroid upper;

        upper.setPosition(pos.x + radius,
                          pos.y + radius);

        auto aa = std::lower_bound(asteroids.begin(), asteroids.end(), lower);
        auto bb = std::upper_bound(aa, asteroids.end(), upper);

        for(; aa != bb; aa++) {
            if(aa->empty()) continue;
            if(a.checkCollision(*aa)) {
                a.collide(*aa);
            }
        }
    }

    for(auto& aa : temp2) {
        if(aa.empty()) continue;
        asteroids.push_back(aa);
        remainingAsteroids += 1;
    }

    //std::sort(asteroids.begin(), asteroids.end());


    Vector2D<float> pos;
    float radius;

    data.player->getPosition(pos.x, pos.y);
    data.player->getCollisionRadius(radius);

    Asteroid lower;

    lower.setPosition(pos.x - radius,
                      pos.y - radius);

    Asteroid upper;

    upper.setPosition(pos.x + radius,
                      pos.y + radius);

    auto a = std::lower_bound(asteroids.begin(), asteroids.end(), lower);
    auto b = std::upper_bound(a, asteroids.end(), upper);

    for(; a != b; a++) {
        if(a->empty()) continue;
        if(data.player->checkCollision(*a)) {
            data.gameOver = true;
        }
    }

}

void Map::draw(const UpdateData& data)
{
    for(auto& a : asteroids) {
        a.draw(data);
    }

    for(auto& a : bullets) {
        a.draw(data);
    }
}

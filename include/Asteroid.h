#ifndef Asteroid_H
#define Asteroid_H

#include <cstdint>
#include <map>
#include <vector>

#include "UpdateData.h"
#include "GameObject.h"


enum class AsteroidType : uint8_t
{
    SMALL,
    BIG,
    COUNT,
    NONE
};

class Asteroid : public GameObject
{
    public:
        Asteroid();
        Asteroid(const char* path);
        Asteroid(const AsteroidType);
        Asteroid(const U_Sprite&, const AsteroidType);
        virtual ~Asteroid();

        virtual void update(const UpdateData& data) override;
        virtual void unUpdate(const UpdateData& data) override;
        virtual std::vector<Asteroid> breakInPieces(const GameObject& obj);
        virtual void collide(Asteroid&);

        static constexpr float maxSpeed = 0.0f;
        static constexpr float speedTowardsPlayer = 5.0f;
        friend bool operator<(const Asteroid& a, const Asteroid& b);
    protected:
        AsteroidType type;
    private:

};

bool operator<(const Asteroid& a, const Asteroid& b);
#endif // Asteroid_H

#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "U_Sprite.h"

#define DONT_MAKE_A_DLL
#include "Framework.h"

class Player : public GameObject
{
    public:
        Player();
        Player(float x, float y, float angle);
        ~Player();

        virtual void update(const UpdateData& data) override;
        virtual void unUpdate(const UpdateData& data) override;
        //virtual void draw() const override;
        //virtual void draw(const UpdateData& data) const override;

        constexpr static const float defAccel = 5.0f;
        constexpr static const float maxSpeed = 10.0f;
    protected:
        Vector2D<float> motionDelta;
    private:

};

#endif // PLAYER_H

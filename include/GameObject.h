#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "U_Sprite.h"
#include "UpdateData.h"
#include "Vector2d.h"

class UpdateData;

class GameObject
{
    public:
        GameObject();
        GameObject(const char* spritePath);
        GameObject(const U_Sprite&);
        virtual ~GameObject();

        virtual void update(const UpdateData&) = 0;
        virtual void unUpdate(const UpdateData&) = 0;
        virtual void draw() const;
        virtual void draw(const UpdateData& data) const;

        virtual bool checkCollision(const GameObject& another) const;

        virtual bool empty() const;
        virtual void setEmpty(bool flag);

        const U_Sprite& getU_Sprite() const;

        virtual void setCollisionRadius(float) final;
        virtual void getCollisionRadius(float&) const final;

        virtual void setSprite(const char* path) final;
        virtual void setSprite(const U_Sprite&) final;

        virtual void setPosition(float x, float y) final;
        virtual void getPosition(float &x, float &y) const final;

        virtual void setMiddlePoint(float x, float y) final;
        virtual void getMiddlePoint(float &x, float &y) const final;

        virtual void setMovement(const Vector2D<float>& speed,
                                 const Vector2D<float>& acceleration) final;

        virtual void getMovement(Vector2D<float>& speed,
                                 Vector2D<float>& acceleration) const final;

        virtual void setRotation(float speed,
                                 float acceleration = 0.0f,
                                 float start_angle = 0.0f) final;

        virtual void getRotation(float& angle,
                                 float& speed,
                                 float& acceleration) const final;

        virtual void setMass(float mass) final;

        virtual void getMass(float& mass) const final;

        bool operator<(const GameObject& b)
        {
            Vector2D<float> a_pos;
            Vector2D<float> b_pos;

            this->getPosition(a_pos.x, a_pos.y);
            b.getPosition(b_pos.x, b_pos.y);

            return a_pos < b_pos;
        }
    protected:
        // Your collision boundaries may differ from the sprite ones
        float collisionRadius{};


        Vector2D<float> velocity{};
        Vector2D<float> accel{};
        Vector2D<float> position{};

        float mass{};

        U_Sprite sprite_;

        float sprite_angle{}; // Reserved for future use
        float angular_velocity{};
        float angular_accel{};  // once ability to rotate sprites is there.
    private:
};


#endif // GAMEOBJECT_H

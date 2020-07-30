#pragma once

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "U_Sprite.h"

struct UpdateData;

class BackGround
{
    public:
        BackGround();
        BackGround(const char* path);
        ~BackGround();
        void draw(const UpdateData&);
        void setSprite(const U_Sprite&);
        void setSprite(const char*);
    protected:
        U_Sprite background_;
};

#endif // BACKGROUND_H

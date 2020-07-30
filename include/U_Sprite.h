#pragma once

#ifndef U_SPRITE_H
#define U_SPRITE_H

#include <cstddef>

#define DONT_MAKE_A_DLL
#include "Framework.h"

// Actually useful sprite, instead of size variables floating around
class U_Sprite
{
    public:
        U_Sprite();
        U_Sprite(const char* path);

        U_Sprite(U_Sprite&&) = default;
        U_Sprite(const U_Sprite&);
        U_Sprite& operator=(const U_Sprite&);
        virtual ~U_Sprite();
        virtual void draw() const;
        virtual bool empty() const;

        virtual void setEmpty(bool flag);

        virtual void getMiddlePoint(int& xx, int& yy) const;

        const Sprite* getSprite() const;
        int x, y;
        int w, h;
    protected:
        Sprite* sprite_;
    private:
        size_t* refcount = nullptr;

        void setRefCounter(size_t *ptr);
        size_t* getRefCounter() const;
        void decreaseRefCount() const;
        void increaseRefCount() const;
        void freeSprite();
        bool not_empty;
};

#endif // U_SPRITE_H

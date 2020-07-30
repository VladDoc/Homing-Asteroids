#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "U_Sprite.h"


U_Sprite::U_Sprite() : x(0), y(0), w(0), h(0),
                       sprite_(nullptr), not_empty(false)
{

}

bool U_Sprite::empty() const
{
    return !not_empty;
}

U_Sprite::U_Sprite(const char* path) : x(0), y(0), w(0), h(0), sprite_(nullptr)
{
    sprite_ = createSprite(path);
    if(!sprite_) {
        fprintf(stderr, "Unable to load sprite %s", path);

        exit(0xDEAD);
    }
    // sets w and h
    getSpriteSize(sprite_, this->w, this->h);

    // If out of memory bad_alloc will raise anyway
    // Although it's very unlikely
    this->refcount = new size_t;
    *refcount = 0;
    not_empty = true;
}

U_Sprite::~U_Sprite()
{
    freeSprite();
}

// Copy
U_Sprite::U_Sprite(const U_Sprite& a) :
    x(a.x), y(a.y), w(a.w), h(a.h), sprite_(a.sprite_), not_empty(a.not_empty)
{
    this->refcount = a.getRefCounter();

    // This will increase both
    this->increaseRefCount();
}

U_Sprite& U_Sprite::operator=(const U_Sprite& a)
{
    x = a.x;
    y = a.y;
    w = a.w;
    h = a.h;
    sprite_ = a.sprite_;
    not_empty = a.not_empty;

    refcount = a.getRefCounter();

    this->increaseRefCount();
    return *this;
}

void U_Sprite::draw() const
{
    if(!this->empty()) {
        drawSprite(sprite_, this->x, this->y);
    }
}

const Sprite* U_Sprite::getSprite() const
{
    return sprite_;
}

void U_Sprite::getMiddlePoint(int& xx, int& yy) const
{
    xx = this->x + this->w / 2;
    yy = this->y + this->h / 2;
}


void U_Sprite::increaseRefCount() const
{
    if(refcount) {
        *refcount += 1;
    }
}

void U_Sprite::decreaseRefCount() const
{
    if(!refcount) return;
    if(*refcount > 0) *refcount -= 1;
}

size_t* U_Sprite::getRefCounter() const
{
    return refcount;
}

void U_Sprite::setRefCounter(size_t* ptr)
{
   refcount = ptr;
}

void U_Sprite::setEmpty(bool flag)
{
    not_empty = !flag;
}

void U_Sprite::freeSprite()
{
    // If you are the last owner then delete
    if(!refcount) return;
    if(*refcount > 0) {
        *refcount -= 1;
    } else {
        // No double free
        if(sprite_) destroySprite(sprite_);
        if(refcount) delete refcount;

        sprite_ = nullptr;
        refcount = nullptr;
        not_empty = false;
    }
}

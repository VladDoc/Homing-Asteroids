#include "BackGround.h"
#include "Vector2d.h"
#include "UpdateData.h"
#include "Player.h"

static const char* path = "data/bg2.png";

BackGround::BackGround() : background_(path)
{

}


BackGround::BackGround(const char* p) : background_(p)
{

}

BackGround::~BackGround()
{
}

void BackGround::draw(const UpdateData& data)
{
    if(!background_.empty()) {
        Vector2D<float> playerPos;
        float angle;

        data.player->getMiddlePoint(playerPos.x, playerPos.y);

        const Sprite* bgSprite = background_.getSprite();
        for(int i = -2; i <= data.mapBoundaries.x / background_.h + 2; ++i) {
            for(int j = -2; j <= data.mapBoundaries.y / background_.w + 3; ++j) {
                 drawSprite((Sprite*)bgSprite,
                            (j * background_.w - (int)playerPos.x) + data.screenSize.x / 2,
                            (i * background_.h - (int)playerPos.y) + data.screenSize.y / 2);
            }
        }
    }
}

void BackGround::setSprite(const U_Sprite& sprite)
{
    background_ = sprite;
}


void BackGround::setSprite(const char* p)
{
    U_Sprite temp(p);
    background_ = temp;
}

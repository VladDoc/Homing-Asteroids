#include <cstdlib>
#include <cmath>
#include <memory>
#include <ctime>
#include <cassert>
#include <thread>
#include <chrono>
#include <SDL.h>

#define DONT_MAKE_A_DLL
#include "Framework.h"

#include "BackGround.h"
#include "U_Sprite.h"
#include "Asteroid.h"
#include "UpdateData.h"
#include "Player.h"
#include "Constants.h"
#include "stdoutRedirect.h"
#include "Map.h"
#include "InitData.h"


static std::unique_ptr<InitData> initData;

class AsteroidsGame : public Framework {

    std::unique_ptr<BackGround> bg;

    std::unique_ptr<U_Sprite> cursor;

    std::unique_ptr<Map> map;

    UpdateData upData;

    ControlState state;
public:

    virtual void PreInit(int& width, int& height, bool& fullscreen)
    {
        width = initData->windowSize.x;
        height = initData->windowSize.y;
        fullscreen = false;
    }

    // Load up data
    virtual bool Init() {

        showCursor(false);

        bg = std::make_unique<BackGround>();

        upData.player = std::make_unique<Player>();
        upData.player->setMiddlePoint(initData->windowSize.x / 2,
                                      initData->windowSize.y / 2);

        cursor = std::make_unique<U_Sprite>("data/circle.tga");

        upData.mapBoundaries.x = initData->mapSize.x;
        upData.mapBoundaries.y = initData->mapSize.y;
        upData.frametime = 15;

        getScreenSize(upData.screenSize.x, upData.screenSize.y);

        map = std::make_unique<Map>(initData->num_enemies,
                                    initData->num_ammo, upData);

        setWindowPos(8, 27);

        return true;
    }

    virtual void Close()
    {
    }

    // Update
    virtual bool Tick() {

        const auto start = std::chrono::system_clock::now();

        if(upData.gameOver) {
            std::unique_ptr<Map> newMap = std::make_unique<Map>(initData->num_enemies,
                                                                initData->num_ammo, upData);
            map.swap(newMap);

            std::unique_ptr<Player> newPlayer = std::make_unique<Player>();
            upData.player.swap(newPlayer);
            upData.gameOver = false;
        }
        bg->draw(upData);

        upData.player->getMiddlePoint(upData.lastPlayerPos.x, upData.lastPlayerPos.y);

        upData.player->update(upData);

        map->update(upData);

        upData.player->draw(upData);

        map->draw(upData);

        cursor->x = upData.mouseCoords.x - cursor->w / 2;
        cursor->y = upData.mouseCoords.y - cursor->h / 2;

        cursor->draw();

        const auto end = std::chrono::system_clock::now();

        upData.frametime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        auto ms = std::chrono::duration<uint64_t, std::milli>(upData.frametime < 16 ? 16 - upData.frametime : 0);
        std::this_thread::sleep_for(ms);

        upData.frametime = upData.frametime < 16 ? 16 : upData.frametime;
        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
    {
        upData.mouseCoords.x = x;
        upData.mouseCoords.y = y;

        upData.mouseMotionDelta.x = xrelative;
        upData.mouseMotionDelta.y = yrelative;
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

        if (button == FRMouseButton::LEFT && !isReleased)
        {
            upData.controls.fire = true;
        }

    }

    virtual void onKeyPressed(FRKey k) {

        onKey(k);
    }

    virtual void onKeyReleased(FRKey k)
    {
        switch (k)
        {
        case FRKey::LEFT:
            upData.controls.turnLeft = false;
            break;
        case FRKey::RIGHT:
            upData.controls.turnRight = false;
            break;
        case FRKey::DOWN:
        case FRKey::S:
            upData.controls.goBackward = false;
            break;
        case FRKey::UP:
        case FRKey::W:
            upData.controls.goForward = false;
            break;
        case FRKey::SPACE:
            upData.controls.fire = false;
            break;
        case FRKey::A:
            upData.controls.strafeLeft = false;
            break;
        case FRKey::D:
            upData.controls.strafeRight = false;
            break;
        }
    }

protected:
    void onKey(FRKey k)
    {
        switch (k)
        {
        case FRKey::LEFT:
            upData.controls.turnLeft = true;
            break;
        case FRKey::RIGHT:
            upData.controls.turnRight = true;
            break;
        case FRKey::DOWN:
        case FRKey::S:
            upData.controls.goBackward = true;
            break;
        case FRKey::UP:
        case FRKey::W:
            upData.controls.goForward = true;
            break;
        case FRKey::SPACE:
            upData.controls.fire = true;
            break;
        case FRKey::A:
            upData.controls.strafeLeft = true;
            break;
        case FRKey::D:
            upData.controls.strafeRight = true;
            break;
        }

    }


    virtual const char* GetTitle() override
    {
        return "Evil Homing Asteroids of Death";
    }
};

int main(int argc, char** argv)
{
    switchStderr("errors.txt");
    initData = std::make_unique<InitData>(argc, argv);
    return run(new AsteroidsGame);
}

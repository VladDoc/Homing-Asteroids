#include "util.h"
#include "GameObject.h"

Vector2D<float> util::getCamera(GameObject& obj,
                                Vector2D<int> screenSize,
                                Vector2D<int> mapBoundaries)
{
    Vector2D<float> camera;

    obj.getMiddlePoint(camera.x, camera.y);

    camera.x = util::clamp(camera.x,
                           (float)screenSize.x / 2,
                           (float)mapBoundaries.x - screenSize.x / 2);

    camera.y = util::clamp(camera.y,
                           (float)screenSize.y / 2,
                           (float)mapBoundaries.y - screenSize.y / 2);

    return camera;
}
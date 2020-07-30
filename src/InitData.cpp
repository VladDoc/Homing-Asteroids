#include "InitData.h"

#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <cassert>

#include "util.h"


static char* getCmdOption(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

static bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}



InitData::InitData(int argc, char** argv)
    : windowSize(800, 600),
      mapSize(800, 600),
      num_enemies(10),
      num_ammo(3)
{
    const char* windowSize_ = getCmdOption(argv, argv + argc, "-window");
    int check = 0;

    if(windowSize_) {
        check = sscanf(windowSize_, "%dx%d", &windowSize.x, &windowSize.y);
        assert(check == 2);
    }

    const char* mapSize_ = getCmdOption(argv, argv + argc, "-map");

    if(mapSize_) {
        check = sscanf(mapSize_, "%dx%d", &mapSize.x, &mapSize.y);
        assert(check == 2);

        mapSize.x = util::clamp(mapSize.x, windowSize.x, 10000);
        mapSize.y = util::clamp(mapSize.y, windowSize.y, 10000);

    }

    const char* num_enemies_ = getCmdOption(argv, argv + argc, "-num_enemies");

    if(num_enemies_) {
        check = sscanf(num_enemies_, "%d", &num_enemies);
        assert(check == 1);
    }

    const char* num_ammo_ = getCmdOption(argv, argv + argc, "-num_ammo");

    if(num_ammo_) {
        check = sscanf(num_ammo_, "%d", &num_ammo);
        assert(check == 1);
    }
}

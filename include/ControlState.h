#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H


struct ControlState
{
    bool turnLeft{};
    bool turnRight{};

    bool goForward{};
    bool goBackward{};

    bool strafeLeft{};
    bool strafeRight{};

    bool fire{};
};

#endif // CONTROLSTATE_H

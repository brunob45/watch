#ifndef __STATE_HPP__
#define __STATE_HPP__

namespace STATE
{
enum state_t
{
    SLEEP,
    DISP,
    PROG
};

void nextState(volatile state_t &state)
{
    switch (state)
    {
    case SLEEP:
        state = DISP;
        break;

    case DISP:
        state = PROG;
        break;

    case PROG:
        state = DISP;
        break;
    }
}
}

#endif
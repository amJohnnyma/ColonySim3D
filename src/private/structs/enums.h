#ifndef ENUMS_H
#define ENUMS_H

enum State 
{
    IDLE,
    RUNNING,
    PAUSED,
    STOPPED
};
enum Event
{
    START,
    STOP,
    PAUSE,
    UNPAUSE
};

#endif
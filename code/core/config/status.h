#ifndef __STATUS_H
#define __STATUS_H

enum Robot_State
{
    offline = 0,
    normal,
    over_temperature,
    fault,
};

#endif
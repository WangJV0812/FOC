#ifndef __SOFT_TIMER_H
#define __SOFT_TIMER_H

#include "bsp_tim.h"
#include "status.h"

struct Function_Handler
{
    enum State{
        normal = 0,
        overtime,
        unlinkable,
        finish
    };

    /* a simplified function point type */
    typedef void (* fp) (int16_t argc, const char *argv[]);

    /* the parameter _repetitions means that how much thims callback funtion to be executed. */
    /*  In particular, if _repetitions is -1, callback function will be executed indefinitely */
    Function_Handler(fp _callback_function_p,
                        int32_t _repetitions,
                        int _cyclicality);
    ~Function_Handler();

    uint32_t run();
private:
    State status_;
    /* initlize the callback function pointer as nullptr */
    fp callback_function_p_ = nullptr;
    int32_t repetitions_;
    int cyclicality_;
    int32_t counter_;
};

class Soft_Timer
{
public:
    /* Maximum number of function can be managed by soft timer */
    Soft_Timer(size_t _queue_size);


private:
    uint64_t basic_counter = 0;
    

};




#endif

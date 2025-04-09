#include "module.h"

Function_Handler::Function_Handler(fp _callback_function_p, int32_t _repetitions, int _cyclicality)
{
    if (_callback_function_p != nullptr) {
        this->callback_function_p_ = _callback_function_p;
    } else {
        status_ = Function_Handler::State::unlinkable;
    }

    this->repetitions_ = _repetitions;
    this->cyclicality_ = _cyclicality;

    status_ = Function_Handler::State::normal;
}

uint32_t Function_Handler::run()
{
    if (this->status_ != normal)
        return -1;

    this->callback_function_p_(0, nullptr);

    if (this->cyclicality_ != -1) {
        /* do not work in the continual mode */
        if (this->counter_ >= this->cyclicality_) {
            /* Counter set value is refreshed */
            this->status_ = finish;
        } else {
            this->counter_ += 1;
        }
        return this->counter_;
    }

    /* work in the continual mode */
    this->counter_ = -1;
    return -1;
}

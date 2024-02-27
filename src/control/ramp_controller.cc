#include "ramp_controller.hpp"

#include <math.h>

namespace Ramp
{

    fp32 Ramp_ctrller::ramp_calc(fp32 input, fp32 diff) {
        if (input * output < 0) {
            output = 0;
            return output;
        } else if (fabs(input - output) < diff) {
            output = input;
        }

        if (output > input) {
            output -= diff;
        } else if (output < input) {
            output += diff;
        }
        return output;
    }

    Ramp_ctrller::Ramp_ctrller() {
        output = 0.0f;
        diff = 0;
    }

    Ramp_ctrller::~Ramp_ctrller() {
    }

}  // namespace Ramp

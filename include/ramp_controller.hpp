#ifndef __RAMP_CONTROLLER__
#define __RAMP_CONTROLLER__
#include "types.hpp"

namespace Ramp
{
    class Ramp_ctrller
    {
       public:
        Ramp_ctrller();
        ~Ramp_ctrller();
        fp32 ramp_calc(fp32 input, fp32 diff);

       private:
       public:
        fp32 output;  // 输出数据
        fp32 diff;

       private:
    };

}  // namespace Ramp

#endif

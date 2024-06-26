#include "user_lib.hpp"

#include "thread"
#include "chrono"

namespace UserLib
{
    void sleep_ms(uint32_t dur) {
        std::this_thread::sleep_for(std::chrono::milliseconds(dur));
    }

    fp32 rad_format(fp32 ang) {
        fp32 ans = fmodf(ang + M_PIf, M_PIf * 2.f);
        return (ans < 0.f) ? ans + M_PIf : ans - M_PIf;
    }

    void RealRad::update(fp32 ref) {
        if (last < -3.f && ref > 3.f) {
            count--;
        } else if (last > 3.f && ref < -3.f) {
            count++;
        }
        last = ref;
        now = (fp32)count * 2.f * M_PIf + ref;
    }

}  // namespace UserLib

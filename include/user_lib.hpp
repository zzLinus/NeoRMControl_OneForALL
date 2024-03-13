#include <cmath>

#include "types.hpp"

namespace UserLib
{
    fp32 rad_format(fp32 ang);

    template<typename T>
    void unpack(T &t, void *ptr) {
        for (size_t i = 0; i < sizeof(T); i++) {
            *((uint8_t *)&t + i) = *((uint8_t *)ptr + i);
        }
    }

    class RealRad
    {
       public:
        void update(fp32 ref);

       public:
        int count = 0;
        fp32 now = 0.0;
        fp32 last = 0.0;
    };
}  // namespace UserLib

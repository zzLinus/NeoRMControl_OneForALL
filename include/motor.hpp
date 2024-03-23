#ifndef __MOTOR__
#define __MOTOR__

#include "pid_controller.hpp"
#include "types.hpp"
#include "can.hpp"
#include "device/deviece_base.hpp"
#include <linux/can.h>

namespace Hardware
{

    struct motor_can_message
    {
        uint16_t ecd = 0;
        int16_t speed_rpm = 0;
        int16_t given_current = 0;
        uint8_t temperate = 0;
        int16_t last_ecd = 0;
    };

    class Motor : public Device::DeviceBase
    {
       public:
        explicit Motor(const Pid::Pid_config &config) : pid_ctrler(config) {};
        void unpack(const can_frame &frame);
        ~Motor() = default;

       private:
       public:
        Pid::Pid_position pid_ctrler;
        // TODO: can bus api related
        motor_can_message motor_measure;

        fp32 accel = 0.f;
        fp32 speed = 0.f;
        fp32 speed_set = 0.f;

        int16_t give_current = 0;

       private:
    };

    can_frame get_frame(canid_t can_id, const std::vector<Motor> & mots);

    template<typename ...Args>
    can_frame get_frame(canid_t can_id, const Motor &head, Args &&...args) {
        can_frame frame{};
        frame.can_id = can_id;
        frame.can_dlc = 8;
        std::vector<const Motor*> mot_list = {&head, &args...};
        for(int i = 0; i < mot_list.size() && i < 4; i++) {
            frame.data[i << 1] = (mot_list[i]->give_current >> 8);
            frame.data[i << 1 | 1] = (mot_list[i]->give_current & 0xFF);
        }
        return frame;
    }
}  // namespace Hardware

#endif

#ifndef NEW_INFANTRY_MOTOR_SENDER_H
#define NEW_INFANTRY_MOTOR_SENDER_H

#include <vector>
#include <optional>
#include <thread>
#include <memory>
#include <type_traits>
#include "motor.hpp"
#include "can.hpp"

namespace Hardware
{
    struct Motor_sender
    {
        std::optional<canid_t> can_id;
        std::vector<Motor*> pMotors;
        Can_interface can;
        std::unique_ptr<std::jthread> thread;

        void send();
        void init_can(canid_t id,const char* can_channel);

        template<class... Args>
        void addMotor(Motor& m,canid_t receive_id,Args&&... args){
            pMotors.push_back(&m);
            can.register_callback_key(receive_id,[&m](const auto &frame) { m.unpack(frame); });
            if constexpr (sizeof...(args))
                addMotor(std::forward<Args>(args)...);
        }

        template<class... Args> requires ((std::is_same_v<Hardware::Motor,std::decay_t<Args>>) && ...)
        void addMotor(Motor& m,Args&&... args)
        {
            pMotors.push_back(&m);
            if constexpr (sizeof...(args))
                addMotor(std::forward<Args>(args)...);
        }

    };


}



#endif //NEW_INFANTRY_MOTOR_SENDER_H

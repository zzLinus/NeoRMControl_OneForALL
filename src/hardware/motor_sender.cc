#include "motor_sender.hpp"

void Hardware::Motor_sender::send() {
    can_frame frame{};
    frame.can_id = can_id.value();
    frame.can_dlc = 8;
    for(int i = 0; i < pMotors.size() && i < 4; i++) {
        frame.data[i << 1] = (pMotors[i]->give_current >> 8);
        frame.data[i << 1 | 1] = (pMotors[i]->give_current & 0xFF);
    }
    can.send(frame);
    LOG_INFO("send info:");
    for (auto p : pMotors)
        LOG_INFO("%-8d ",p->give_current);
    LOG_INFO("to can id %x\n",can_id.value());
}

void Hardware::Motor_sender::init_can(canid_t id, const char *can_channel) {
    can.init(can_channel);
    thread = std::make_unique<std::jthread>(&Can_interface::task,&can);
    can_id = id;
}
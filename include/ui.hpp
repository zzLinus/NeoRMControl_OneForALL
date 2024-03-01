#ifndef __UI__
#define __UI__

#include <chrono>
#include <functional>
#include <memory>
#include <raylib-cpp.hpp>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"  // Required for GUI controls

//
#include "kb_ctrl.hpp"
#include "robot.hpp"
#include "types.hpp"

namespace Ui
{
    class raylib_ui
    {
       public:
        using InputHandler = std::function<fp32(std::shared_ptr<Robot::Robot_set>, Types::Kb_event, fp32)>;

        raylib_ui() = delete;
        raylib_ui(std::shared_ptr<Robot::Robot_set> p_robot_set, const InputHandler& event_handler);
        ~raylib_ui();
        void init();  // WARN: some how, the raylib window allocation must hapen after the constructors;
        bool render();
        inline void draw_motor_speed();
        inline void draw_menu();
        inline void handle_kbevent();

       private:
       public:
        Types::debug_info_t* debug;

       private:
        std::shared_ptr<Robot::Robot_set> p_robot_set;
        float max_speed = 2.5;

        fp32 cur_speed_x;
        fp32 cur_speed_y;
        InputHandler input_handler;
        raylib::Window* w;

        uint64_t last;
        uint64_t last_x;

        uint32_t screenW;
        uint32_t screenH;
        raylib::Color textColor;
    };
}  // namespace Ui

#endif

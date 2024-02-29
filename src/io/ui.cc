#include "ui.hpp"

namespace Ui
{
    Ncurses_ui::Ncurses_ui(const InputHandler &event_handler) {
        input_handler = event_handler;
        debug = new Types::debug_info_t;
        cur_speed_x = 0;
        cur_speed_y = 0;
    }

    Ncurses_ui::~Ncurses_ui() {
        delete debug;
    }

    void Ncurses_ui::init() {
        w = new raylib::Window(800, 450, "Infantry");
        SetTargetFPS(60);
        last = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
        last_x =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
    }

    bool Ncurses_ui::render() {
        // Initialization
        if (w->ShouldClose())
            return false;
        raylib::Color textColor(RED);

        uint64_t now =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        float spdslider = 2.5;

        if (IsKeyDown(KEY_W)) {
            textColor.DrawText(" Hi Mom", 50, 50, 20);
            cur_speed_y = input_handler(Types::Kb_event::UP, spdslider);
            last = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now()
                           .time_since_epoch())
                       .count();
        }
        if (IsKeyDown(KEY_A)) {
            cur_speed_x = input_handler(Types::Kb_event::LEFT, spdslider);
            last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now()
                             .time_since_epoch())
                         .count();
            textColor.DrawText(" Hi Mom", 20, 70, 20);
        }
        if (IsKeyDown(KEY_S)) {
            cur_speed_x = input_handler(Types::Kb_event::RIGHT, spdslider);
            last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now()
                             .time_since_epoch())
                         .count();
            textColor.DrawText(" Hi Mom", 80, 70, 20);
        }
        if (IsKeyDown(KEY_R)) {
            cur_speed_y = input_handler(Types::Kb_event::DOWN, spdslider);
            last = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now()
                           .time_since_epoch())
                       .count();
            textColor.DrawText(" Hi Mom", 50, 90, 20);
        }

        // NOTE: dec velocity over time
        if (now - last > 300) {
            cur_speed_y = input_handler(Types::Kb_event::STOP_Y, 0);
            last = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now()
                           .time_since_epoch())
                       .count();
        }
        if (now - last_x > 300) {
            cur_speed_x = input_handler(Types::Kb_event::STOP_X, 0);
            last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now()
                             .time_since_epoch())
                         .count();
            printf("hi mom\n");
        }

        textColor.DrawText("Hi! Mom", 10, 10, 20);

        EndDrawing();
        return true;
    }
}  // namespace Ui

#include "ui.hpp"

namespace Ui
{
    raylib_ui::raylib_ui(std::shared_ptr<Robot::Robot_set> robot_set, const InputHandler &event_handler) {
        debug = new Types::debug_info_t;
        input_handler = event_handler;
        p_robot_set = robot_set;
        cur_speed_x = 0;
        cur_speed_y = 0;
        screenW = 800;
        screenH = 600;
        textColor = raylib::Color(RED);
    }

    raylib_ui::~raylib_ui() {
        delete debug;
    }

    void raylib_ui::init() {
        w = new raylib::Window(screenW, screenH, "Infantry");
        SetTargetFPS(60);
        last = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
        last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now().time_since_epoch())
                     .count();
    }

    inline void raylib_ui::draw_menu() {
        DrawFPS(screenW - 100, 10);
        DrawLine(500, 0, 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.6f));
        DrawRectangle(500, 0, GetScreenWidth() - 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.3f));
        textColor.DrawText(" Hi Mom", 10, 10, 20);

        Vector2 dir_center = { 100, 130 };
        DrawRectangle(dir_center.x, dir_center.y - 20, 20, 20, IsKeyDown(KEY_W) ? RED : LIGHTGRAY);
        DrawRectangle(dir_center.x, dir_center.y + 20, 20, 20, IsKeyDown(KEY_R) ? RED : LIGHTGRAY);
        DrawRectangle(dir_center.x - 20, dir_center.y, 20, 20, IsKeyDown(KEY_A) ? RED : LIGHTGRAY);
        DrawRectangle(dir_center.x + 20, dir_center.y, 20, 20, IsKeyDown(KEY_S) ? RED : LIGHTGRAY);
    }

    inline void raylib_ui::draw_motor_speed() {
        // NOTE: Draw Slidebar
        GuiSliderBar((Rectangle){ 600, 200, 120, 20 }, "Max speed", NULL, &max_speed, 0, 2.5);

        Vector2 center = { 280, 130 };
        float vx_per = (p_robot_set->vx_set / 2.5) * 360;
        float vy_per = (p_robot_set->vy_set / 2.5) * 360;
        if (vx_per > 0) {
            DrawRing(center, 20.0f, 45.0f, vx_per, 0, 0, Fade(MAROON, 0.8f));
        } else {
            DrawRing(center, 20.0f, 45.0f, vx_per, 0, 0, Fade(LIME, 0.8f));
        }

        if (vy_per > 0) {
            DrawRing(Vector2{ center.x + 0, center.y + 100 }, 20.0f, 45.0f, vy_per, 0, 0, Fade(MAROON, 0.8f));
        } else {
            DrawRing(Vector2{ center.x + 0, center.y + 100 }, 20.0f, 45.0f, vy_per, 0, 0, Fade(LIME, 0.8f));
        }
    }
    inline void raylib_ui::handle_kbevent() {
        uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
        if (IsKeyDown(KEY_W)) {
            cur_speed_y = input_handler(p_robot_set, Types::Kb_event::UP, max_speed);
            last = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now().time_since_epoch())
                       .count();
        }
        if (IsKeyDown(KEY_A)) {
            cur_speed_x = input_handler(p_robot_set, Types::Kb_event::LEFT, max_speed);
            last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now().time_since_epoch())
                         .count();
        }
        if (IsKeyDown(KEY_S)) {
            cur_speed_x = input_handler(p_robot_set, Types::Kb_event::RIGHT, max_speed);
            last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now().time_since_epoch())
                         .count();
        }
        if (IsKeyDown(KEY_R)) {
            cur_speed_y = input_handler(p_robot_set, Types::Kb_event::DOWN, max_speed);
            last = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now().time_since_epoch())
                       .count();
        }

        // NOTE: dec velocity over time
        if (now - last > 30) {
            cur_speed_y = input_handler(p_robot_set, Types::Kb_event::STOP_Y, 0);
            last = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now().time_since_epoch())
                       .count();
        }
        if (now - last_x > 30) {
            cur_speed_x = input_handler(p_robot_set, Types::Kb_event::STOP_X, 0);
            last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::high_resolution_clock::now().time_since_epoch())
                         .count();
        }
    }

    bool raylib_ui::render() {
        // Initialization
        if (w->ShouldClose())
            return false;

        BeginDrawing();
        ClearBackground(Color{ 6, 0, 37, 255 });

        draw_menu();
        draw_motor_speed();
        handle_kbevent();

        EndDrawing();
        return true;
    }
}  // namespace Ui

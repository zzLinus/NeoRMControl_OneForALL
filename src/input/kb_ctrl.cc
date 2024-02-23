#include "kb_ctrl.hpp"

#include <bits/chrono.h>

namespace Input
{
    Kb_ctrl::Kb_ctrl()
    {
        // HACK:键盘控制斜波函数初始化
        kb_vx_ramp = new Ramp::Ramp_ctrller();
        kb_vy_ramp = new Ramp::Ramp_ctrller();

        rc_ctrl = new Types::RC_ctrl_t();
        debug = new Types::debug_info_t;
    }

    Kb_ctrl::~Kb_ctrl()
    {
        delete kb_vx_ramp;
        delete kb_vy_ramp;
        delete debug;
        delete rc_ctrl;
    }

    void Kb_ctrl::task()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto screen = ImTui_ImplNcurses_Init(true);
        ImTui_ImplText_Init();
        uint64_t last = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch())
                            .count();
        uint64_t last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::high_resolution_clock::now().time_since_epoch())
                              .count();
        // NOTE: UI and keyboard event
        while (true)
        {
            float spdslider;
            uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::high_resolution_clock::now().time_since_epoch())
                               .count();
            ImTui_ImplNcurses_NewFrame();
            ImTui_ImplText_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(4, 27), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 15.0), ImGuiCond_Once);
            ImGui::Begin("Debug");
            ImGui::Text(
                "Time per frame %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 0, 1));
            ImGui::Text("velocity : vx vy wz");
            ImGui::Text(" %f %f %f", debug->vx, debug->vy, debug->wz);
            ImGui::SliderFloat("speed percent", &spdslider, 0.0f, Config::OPEN_LOOP_MAX_SPEED, "ratio = %.3f");
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "can pkg : %0lx", debug->pkg);

            ImGui::Text("time %lu %lu %lu", last, now, now - last);
            ImGui::Text("ramp %f", kb_vy_ramp->output);

            ImGui::Text("Hi mon! %d: ", debug->err);
            ImGui::Text(
                "candump id :%x\n dlc : %d \n%02x %02x %02x %02x %02x %02x %02x %02x",
                debug->can_f.can_id,
                debug->can_f.can_dlc,
                debug->can_f.data[0],
                debug->can_f.data[1],
                debug->can_f.data[2],
                debug->can_f.data[3],
                debug->can_f.data[4],
                debug->can_f.data[5],
                debug->can_f.data[6],
                debug->can_f.data[7]);

            if (ImGui::IsKeyPressed('w', true) && now - last > 70)
            {
                rc_ctrl->key.speed_y = kb_vy_ramp->ramp_calc(spdslider, spdslider / 10);
                last = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
            }
            if (ImGui::IsKeyPressed('r', true) && now - last > 70)
            {
                rc_ctrl->key.speed_y = kb_vy_ramp->ramp_calc(-spdslider, spdslider / 10);
                last = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
            }

            if (ImGui::IsKeyPressed('a', true) && now - last_x > 70)
            {
                rc_ctrl->key.speed_x = kb_vx_ramp->ramp_calc(-spdslider, spdslider / 10);
                last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch())
                             .count();
            }
            if (ImGui::IsKeyPressed('s', true) && now - last_x > 70)
            {
                rc_ctrl->key.speed_x = kb_vx_ramp->ramp_calc(spdslider, spdslider / 10);
                last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch())
                             .count();
            }

            // NOTE: dec velocity over time
            if (now - last > 300)
            {
                rc_ctrl->key.speed_y = kb_vy_ramp->ramp_calc(0, spdslider / 4);
                last = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
            }
            if (now - last_x > 300)
            {
                rc_ctrl->key.speed_x = kb_vx_ramp->ramp_calc(0, spdslider / 4);
                last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch())
                             .count();
            }

            // FIXME:
            if (ImGui::IsKeyPressed('q', true))
            {
                rc_ctrl->key.q = 0x1;
                rc_ctrl->key.f = 0x0;
            }
            if (ImGui::IsKeyPressed('f', true))
            {
                rc_ctrl->key.f = 0x1;
                rc_ctrl->key.q = 0x0;
            }

            ImGui::PopStyleColor(1);

            ImGui::SameLine();

            ImGui::End();

            ImGui::SetNextWindowPos(ImVec2(30, 5), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 10.0), ImGuiCond_Once);
            ImGui::Begin("Chassis info");
            ImGui::Text("Current direction:");
            if (rc_ctrl->key.speed_y > 0)
            {
                ImGui::Text("  ^");
            }
            else
            {
                ImGui::Text("");
            }
            if (rc_ctrl->key.speed_x < 0)
            {
                ImGui::Text("<");
            }
            else if (rc_ctrl->key.speed_x > 0)
            {
                ImGui::Text("    >");
            }
            else
            {
                ImGui::Text("");
            }
            if (rc_ctrl->key.speed_y < 0)
            {
                ImGui::Text("  v");
            }
            else
            {
                ImGui::Text("");
            }

            // [3]  [0]
            //
            // [2]  [1]
            int index[] = { 3, 0, 2, 1 };

            for (int i = 0; i < 4; i++)
            {
                if (debug->wheel_speed[index[i]] != 0.0)
                {
                    debug->wheel_speed[index[i]] /= 2 * spdslider;
                }

                if (debug->wheel_speed[index[i]] > 0)
                {
                    ImGui::ProgressBar(debug->wheel_speed[index[i]], ImVec2(10.0f, 1.0f));
                }
                else
                {
                    ImGui::ProgressBar2(-debug->wheel_speed[index[i]], ImVec2(10.0f, 1.0f));
                }
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("Motor %d", index[i]);
                if (i % 2 == 0)
                {
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                }
                else
                {
                    ImGui::Text("");
                }
            }

            ImGui::End();

            ImGui::Render();

            ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
            ImTui_ImplNcurses_DrawScreen();
        }

        ImTui_ImplText_Shutdown();
        ImTui_ImplNcurses_Shutdown();
    }

}  // namespace Input

#include "kb_ctrl.hpp"

namespace Controller
{
    Kb_ctrl::Kb_ctrl()
    {
        rc_ctrl = new Types::RC_ctrl_t();
        debug = new Types::debug_info_t;
        debug->vx = -1;
        debug->vy = -1;
        debug->wz = -1;
        rc_ctrl->key.speed = 0;
    }

    Kb_ctrl::~Kb_ctrl()
    {
        delete debug;
        delete rc_ctrl;
    }

    void Kb_ctrl::task()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto screen = ImTui_ImplNcurses_Init(true);
        ImTui_ImplText_Init();

        while (true)
        {
            float spdslider;
            ImTui_ImplNcurses_NewFrame();
            ImTui_ImplText_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(4, 27), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 10.0), ImGuiCond_Once);
            ImGui::Begin("/u266bDebug");
            ImGui::Text(
                "Time per frame %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 0, 1));
            ImGui::Text("Debug: vx vy wz");
            ImGui::Text("%f %f %f", debug->vx, debug->vy, debug->wz);
            ImGui::SliderFloat("speed percent", &spdslider, 0.0f, 100.0f, "ratio = %.3f");
            rc_ctrl->key.speed = (spdslider / 100) * 0x03ff;
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "can pkg : %0lx", debug->pkg);

            if (ImGui::IsKeyPressed('w', true))
            {
                rc_ctrl->key.dir = 0x1 << 0;
            }
            if (ImGui::IsKeyPressed('r', true))
            {
                rc_ctrl->key.dir = 0x1 << 1;
            }
            if (ImGui::IsKeyPressed('a', true))
            {
                rc_ctrl->key.dir = 0x1 << 2;
            }
            if (ImGui::IsKeyPressed('s', true))
            {
                rc_ctrl->key.dir = 0x1 << 3;
            }

            ImGui::PopStyleColor(1);

            ImGui::SameLine();

            ImGui::End();

            bool s1, s2, s3;
            ImGui::SetNextWindowPos(ImVec2(30, 5), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 10.0), ImGuiCond_Once);
            ImGui::Begin("Config");
            ImGui::Checkbox("No titlebar", &s1);
            ImGui::SameLine(150);
            ImGui::Checkbox("No scrollbar", &s2);
            ImGui::SameLine(300);
            ImGui::Checkbox("No menu", &s3);

            if (rc_ctrl->key.dir == 0x1)
            {
                ImGui::Text("  ^");
            }
            else
            {
                ImGui::Text("");
            }
            if (rc_ctrl->key.dir == 0x1 << 2)
            {
                ImGui::Text("<");
            }
            else if (rc_ctrl->key.dir == 0x1 << 3)
            {
                ImGui::Text("    >");
            }
            else
            {
                ImGui::Text("");
            }
            if (rc_ctrl->key.dir == 0x1 << 1)
            {
                ImGui::Text("  v");
            }
            else
            {
                ImGui::Text("");
            }

            ImGui::Text(
                "%f %f %f %f",
                debug->wheel_speed[0],
                debug->wheel_speed[1],
                debug->wheel_speed[2],
                debug->wheel_speed[3]);
            if (debug->wheel_speed[0] > 0)
            {
                ImGui::ProgressBar(debug->wheel_speed[0], ImVec2(10.0f, 1.0f));
            }
            else
            {
                ImGui::ProgressBar2(-debug->wheel_speed[0], ImVec2(10.0f, 1.0f));
            }
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Motor 1");
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            if (debug->wheel_speed[1] > 0)
            {
                ImGui::ProgressBar(debug->wheel_speed[1], ImVec2(10.0f, 1.0f));
            }
            else
            {
                ImGui::ProgressBar2(-debug->wheel_speed[1], ImVec2(10.0f, 1.0f));
            }
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Motor 2");
            ImGui::Text("%s", "");
            if (debug->wheel_speed[2] > 0)
            {
                ImGui::ProgressBar(debug->wheel_speed[2], ImVec2(10.0f, 1.0f));
            }
            else
            {
                ImGui::ProgressBar2(-debug->wheel_speed[2], ImVec2(10.0f, 1.0f));
            }
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Motor 3");
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            if (debug->wheel_speed[3] > 0)
            {
                ImGui::ProgressBar(debug->wheel_speed[3], ImVec2(10.0f, 1.0f));
            }
            else
            {
                ImGui::ProgressBar2(-debug->wheel_speed[3], ImVec2(10.0f, 1.0f));
            }
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("Motor 4");

            ImGui::End();

            ImGui::Render();

            ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
            ImTui_ImplNcurses_DrawScreen();
        }

        ImTui_ImplText_Shutdown();
        ImTui_ImplNcurses_Shutdown();
    }
}  // namespace Controller

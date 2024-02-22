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
        rc_ctrl->key.speed = 0x05ff;
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

        int nframes = 0;
        float fval = 1.23f;

        while (true)
        {
            ImTui_ImplNcurses_NewFrame();
            ImTui_ImplText_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(4, 27), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 10.0), ImGuiCond_Once);
            ImGui::Begin("Hello, world!");
            ImGui::Text("NFrames = %d", nframes++);
            ImGui::Text("Mouse Pos : x = %g, y = %g", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
            ImGui::Text(
                "Time per frame %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
            ImGui::Text("Float:");
            ImGui::SameLine();
            ImGui::SliderFloat("##float", &fval, 0.0f, 10.0f);

            ImGui::Text("Debug info : %f %f %f", debug->vx, debug->vy, debug->wz);
            ImGui::Text("%0lx", debug->pkg);

            if (ImGui::IsKeyPressed('w', true))
            {
                rc_ctrl->key.dir = 0x1 << 0;
                ;
            }
            if (ImGui::IsKeyPressed('r', true))
            {
                rc_ctrl->key.dir = 0x1 << 1;
                ;
            }
            if (ImGui::IsKeyPressed('a', true))
            {
                rc_ctrl->key.dir = 0x1 << 2;
                ;
            }
            if (ImGui::IsKeyPressed('s', true))
            {
                rc_ctrl->key.dir = 0x1 << 3;
                ;
            }

            ImGui::Text("Dir : %04x", rc_ctrl->key.dir);
            ImGui::End();

            ImGui::Render();

            ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
            ImTui_ImplNcurses_DrawScreen();
        }

        ImTui_ImplText_Shutdown();
        ImTui_ImplNcurses_Shutdown();
    }
}  // namespace Controller

#include "app.h"
#include "canvas.h"

namespace Demo {

    void RenderApp()
    {
        ImGui::Begin("Demo");

        static Canvas* canvas = new Canvas();
        
        if(ImGui::BeginTabBar("Demo"))
        {
            if(ImGui::BeginTabItem("Canvas"))
            {
                canvas->AddPoint(ImVec2(20,10));
                canvas->AddLine(ImVec2(10, 10), ImVec2(20, 40));
                canvas->Render();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }



        ImGui::End();
    }

}
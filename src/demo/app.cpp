#include "app.h"
#include "canvas.h"
#include "convex_hull.h"

namespace Demo {

    void RenderApp()
    {
        ImGui::Begin("Demo");

        static ConvexHull* convexHull = new ConvexHull();
        
        if(ImGui::BeginTabBar("Demo"))
        {
            if(ImGui::BeginTabItem("Canvas"))
            {
                convexHull->Render();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }



        ImGui::End();
    }

}
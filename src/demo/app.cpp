#include<memory>
#include "app.h"

#include "canvas.h"
#include "convex_hull.h"
#include "shortest_path.h"

namespace Demo {

void RenderApp() {
    ImGui::Begin("Demo");

    static auto convexHull = std::make_unique<ConvexHull>();
    static auto shortestPath = std::make_unique<ShortestPath>();

    if (ImGui::BeginTabBar("Demo")) {
        if (ImGui::BeginTabItem("Convex Hull")) {
            convexHull->Render();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Shortest Path")) {
            shortestPath->Render();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

}  // namespace Demo
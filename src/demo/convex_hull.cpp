#include "convex_hull.h"

#include <algorithm>
#include <queue>

#include "demo_utils.h"

namespace Demo {
ConvexHull::ConvexHull() : points{} {
    this->canvas = new Canvas;

    // returns true if the three points make a counter-clockwise turn
    this->cross = [](const ImVec2 &p0, const ImVec2 &p1,
                     const ImVec2 &p2) -> bool {
        return (p1.x - p0.x) * (p2.y - p0.y) > (p2.x - p0.x) * (p1.y - p0.y);
    };
}

void ConvexHull::Render() {
    static ImGuiIO &io = ImGui::GetIO();
    ImGui::Text("%f FPS", io.Framerate);

    static char input[1024] = 
        "0 0 1 3 2 4 5 5 10 10";
    ImGui::InputText("Input", input, sizeof(input));
    ImGui::SameLine();

    if (ImGui::Button("Plot Points!")) {
        AddPoints(input);
        canvas->SetPoints(points);
    }

    if (ImGui::Button("Start!")) {
        canvas->Clear();
        AddPoints(input);
        canvas->SetPoints(points);

        auto ch = Andrew();

        ch.push_back(ch.front());
        for (int i = 1; i < ch.Size; ++i) {
            canvas->AddLine(ch[i], ch[i - 1]);
        }
    }

    canvas->Render();
}

ImVector<ImVec2> ConvexHull::Andrew() {
    SortPoints();
    ImVector<ImVec2> res;

    /**
     * @brief Construct lowerbound edge 
     */
    for (const auto &pt : points) {
        while (res.Size >= 2 &&
               !cross(res[(res.Size - 2)], res[res.Size - 1], pt)) {
            res.pop_back();
        }
        res.push_back(pt);
    }

    /**
     * @brief Construct upperbound edge 
     */
    auto t = res.Size + 1;
    for (int i = points.Size - 1; i >= 0; i--) {
        while (res.size() >= t &&
               !cross(res[res.Size - 2], res[res.Size - 1], points[i])) {
            res.pop_back();
        }
        res.push_back(points[i]);
    }

    res.pop_back(); // remove duplicate starting point
    return res;
}

void ConvexHull::AddPoints(char *__points) {
    this->points.clear();
    auto points_str = Demo::Utils::split(__points, ' ');

    for (int i = 0; i < points_str.size(); i += 2) {
        this->points.push_back(
            ImVec2(std::stoi(points_str[i]), std::stoi(points_str[i + 1])));
    }
}

void ConvexHull::SortPoints() {
    std::sort(points.begin(), points.end(),
              [](ImVec2 &p1, ImVec2 &p2) { return p1.x < p2.x; });
}

}  // namespace Demo
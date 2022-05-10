#pragma once
#include <functional>

#include "canvas.h"

namespace Demo {

class ConvexHull {
   public:
    // Constructors
    ConvexHull();

    void Render();
    void AddPoints(char *numbers);
    void SortPoints();
    ImVector<ImVec2> Andrew();

   private:
    Canvas *canvas;
    ImVector<ImVec2> points;

    std::function<bool(const ImVec2 &p0, const ImVec2 &p1, const ImVec2 &p2)>
        cross;
};

}  // namespace Demo
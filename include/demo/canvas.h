#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#include "math.h"

namespace Demo {

    class Canvas {
    public:
        // Constructors
        Canvas();

        void Render();

        // APIs
        virtual void AddLine(ImVec2 p1, ImVec2 p2);
        virtual void RemoveLine(size_t index);

        virtual void AddPoint(ImVec2 p);
        virtual void RemovePoint(size_t index);
    
    protected:
        float scale;
        ImVector<ImVec2> points;
        ImVector<ImVec4> lines;
        

    };

}
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
        virtual void Clear();
        virtual void AddLine(const ImVec2& p1, const ImVec2& p2);
        virtual void RemoveLine(size_t index);
        virtual void AddPoint(const ImVec2& p);
        virtual void RemovePoint(size_t index);
        virtual void SetPoints(ImVector<ImVec2>& points);
    
    protected:
        float scale;
        ImVector<ImVec2> points;
        ImVector<ImVec4> lines;
        

    };

}
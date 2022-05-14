#include "canvas.h"

namespace Demo {

Canvas::Canvas() : points{}, lines{} {
    // Canvas Constructor writing your code below
    this->scale = 20.0f;
}

void Canvas::Clear() {
    this->points.clear();
    this->lines.clear();
}

void Canvas::AddLine(const ImVec2& p1, const ImVec2& p2) {
    this->lines.push_back(ImVec4(p1.x, -p1.y, p2.x, -p2.y));
}

void Canvas::RemoveLine(size_t index) {
    this->lines.erase(this->lines.begin() + index);
}

void Canvas::AddPoint(const ImVec2& p) {
    this->points.push_back(ImVec2(p.x, -p.y));
}

void Canvas::RemovePoint(size_t index) {
    this->points.erase(this->points.begin() + index);
}

void Canvas::SetPoints(ImVector<ImVec2>& points) {
    for (const auto& p : points) {
        AddPoint(ImVec2(p.x, p.y));
    }
}

void Canvas::Render() {
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;
    static bool opt_enable_context_menu = true;
    static bool opt_enable_point_coordinate = false;
    static bool adding_line = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::SameLine();
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
    ImGui::SameLine();
    ImGui::Checkbox("Enable point coordinate", &opt_enable_point_coordinate);

    // Typically you would use a BeginChild()/EndChild() pair to benefit from a
    // clipping region + own scrolling. Here we demonstrate that this can be
    // replaced by simple offsetting + custom drawing +
    // PushClipRect/PopClipRect() calls. To use a child window instead we could
    // use, e.g:
    //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); //
    //      Disable padding ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50,
    //      50, 50, 255));  // Set a background color
    //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true,
    //      ImGuiWindowFlags_NoMove); ImGui::PopStyleColor();
    //      ImGui::PopStyleVar();
    //      [...]
    //      ImGui::EndChild();

    // Using InvisibleButton() as a convenience 1) it will advance the layout
    // cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 =
        ImGui::GetCursorScreenPos();  // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz =
        ImGui::GetContentRegionAvail();  // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 =
        ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton(
        "canvas", canvas_sz,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered();  // Hovered
    const bool is_active = ImGui::IsItemActive();    // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x,
                        canvas_p0.y + scrolling.y);  // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                     io.MousePos.y - origin.y);

    // // Add first and second point
    // if (is_hovered && !adding_line &&
    // ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    // {
    //     points.push_back(mouse_pos_in_canvas);
    //     points.push_back(mouse_pos_in_canvas);
    //     adding_line = true;
    // }
    // if (adding_line)
    // {
    //     points.back() = mouse_pos_in_canvas;
    //     if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
    //         adding_line = false;
    // }

    // Mouse Wheel Scaling
    if (io.MouseWheel) {
        this->scale += io.MouseWheel;
    }

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse
    // is hovering something etc.
    const float mouse_threshold_for_pan =
        opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right,
                                            mouse_threshold_for_pan)) {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context",
                                    ImGuiPopupFlags_MouseButtonRight);
    if (ImGui::BeginPopup("context")) {
        if (adding_line) points.resize(points.size() - 2);
        adding_line = false;
        if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) {
            this->Clear();
        }
        ImGui::EndPopup();
    }

    if (opt_enable_point_coordinate) {
        for (int n = 0; n < points.Size; n += 1) {
            ImVec2 cen(origin.x + points[n].x * scale - 120,
                       origin.y + points[n].y * scale);
            ImGui::SetCursorPos(cen);
            ImGui::Text("(%02f, %02f)", points[n].x, -points[n].y);
        }
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid) {
        const float GRID_STEP = this->scale;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
             x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                               ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
             y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                               ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
    }

    // Draw lines
    for (int n = 0; n < lines.Size; n += 1) {
        ImVec2 p1((origin.x + lines[n].x * scale),
                  (origin.y + lines[n].y * scale));
        ImVec2 p2((origin.x + lines[n].z * scale),
                  (origin.y + lines[n].w * scale));

        draw_list->AddLine(p1, p2, IM_COL32(255, 255, 0, 255), 2.0f);
    }

    // Draw Points
    for (int n = 0; n < points.Size; n += 1) {
        ImVec2 cen(origin.x + points[n].x * scale,
                   origin.y + points[n].y * scale);
        draw_list->AddCircleFilled(cen, scale / 4.0f,
                                   IM_COL32(255, 255, 0, 255));
    }

    draw_list->PopClipRect();
}

}  // namespace Demo
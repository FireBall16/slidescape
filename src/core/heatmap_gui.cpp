#include "heatmap_gui.h"

#include <algorithm>
#include <vector>

#include "heatmap.h"
#include "heatmap_color_scheme.h"

static void draw_heatmap_colormap_gradient(heatmap_t *heatmap);
static void draw_heatmap_color_buttons(heatmap_t* heatmap);
static void draw_heatmap_color_reset(heatmap_t* heatmap);

void gui_draw_heatmap_menu(heatmap_t *heatmap) {
    ImGui::Begin("Heatmap", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Checkbox("Enable heatmap", &heatmap->enable_heatmap)) {
        set_enable_heatmap(heatmap, heatmap->enable_heatmap);
    };

    if (ImGui::Checkbox("Apply gradient smoothing", &heatmap->apply_gradient_smoothing)) {
        set_heatmap_apply_gradient_smoothing(heatmap, heatmap->apply_gradient_smoothing);
    };

    ImGui::SeparatorText("Colors");
    draw_heatmap_colormap_gradient(heatmap);
    ImGui::Separator();
    draw_heatmap_color_reset(heatmap);

    ImGui::Separator();
    if (ImGui::Button("Sort by stop point")) {
        reset_color_stop_ids(&heatmap->heatmap_colormap);
    }
    ImGui::Separator();

    draw_heatmap_color_buttons(heatmap);
    ImGui::End();
}

static void draw_heatmap_colormap_gradient(heatmap_t* heatmap) {
    heatmap_colormap_t color_map = heatmap->heatmap_colormap;
    std::vector<color_stop_t> stops = {};

    for (int i = 0; i < color_map.color_stop_count; i++)
    {
        if (i == 0 && color_map.color_stops[i].stop_point > 0) {
            color_stop_t new_color_stop = color_map.color_stops[i];
            new_color_stop.stop_point = 0.0f;
            stops.push_back(new_color_stop);
        }

        stops.push_back(color_map.color_stops[i]);

        if (i == color_map.color_stop_count - 1 && color_map.color_stops[i].stop_point < 1.0) {
            color_stop_t new_color_stop = color_map.color_stops[i];
            new_color_stop.stop_point = 1.0f;
            stops.push_back(new_color_stop);
        }
    }

    ImVec2 start_pos = ImGui::GetCursorScreenPos();

    float total_width = 300.0f;
    float height = 40.0f;

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw Checkerboard background to show transparency
    float max_x_checker = start_pos.x + total_width;
    float max_y_checker = start_pos.y + height;
    float checker_size = 8.0f; // Size of each checker square in pixels

    ImU32 color_light = IM_COL32(70, 70, 70, 255);
    ImU32 color_dark = IM_COL32(45, 45, 45, 255);

    for (float y = start_pos.y; y < max_y_checker; y += checker_size) {
        // Check if the final row needs clipping to stay inside the height boundary
        float block_h = (y + checker_size > max_y_checker) ? (max_y_checker - y) : checker_size;

        // Determine alternating row offset
        int row_id_x = static_cast<int>((y - start_pos.y) / checker_size);

        for (float x = start_pos.x; x < max_x_checker; x += checker_size) {
            // Check if the final column needs clipping to stay inside the width boundary
            float block_w = (x + checker_size > max_x_checker) ? (max_x_checker - x) : checker_size;

            int col_id_x = static_cast<int>((x - start_pos.x) / checker_size);

            // Alternate colors based on row and column index
            ImU32 cell_color = ((row_id_x + col_id_x) % 2 == 0) ? color_light : color_dark;

            drawList->AddRectFilled(
                ImVec2(x, y),
                ImVec2(x + block_w, y + block_h),
                cell_color
            );
        }
    }

    // Loop through stops to render adjacent segments
    for (size_t i = 0; i < stops.size() - 1; ++i) {
        color_stop_t &stop_a = stops[i];
        color_stop_t &stop_b = stops[i + 1];

        // Calculate pixel bounds for this sub-gradient
        float min_x = start_pos.x + (stop_a.stop_point * total_width);
        float max_x = start_pos.x + (stop_b.stop_point * total_width);

        ImVec2 p_min(min_x, start_pos.y);
        ImVec2 p_max(max_x, start_pos.y + height);

        // Render horizontal gradient for this segment
        // Top-left, Top-right, Bottom-right, Bottom-left colors
        drawList->AddRectFilledMultiColor(
            p_min,
            p_max,
            IM_COL32(
                stop_a.color[0],
                stop_a.color[1],
                stop_a.color[2],
                stop_a.color[3]
            ), // Top-Left
            IM_COL32(
                stop_b.color[0],
                stop_b.color[1],
                stop_b.color[2],
                stop_b.color[3]
            ), // Top-Right
            IM_COL32(
                stop_b.color[0],
                stop_b.color[1],
                stop_b.color[2],
                stop_b.color[3]
            ), // Bottom-Right
            IM_COL32(
                stop_a.color[0],
                stop_a.color[1],
                stop_a.color[2],
                stop_a.color[3]
            )  // Bottom-Left
        );
    }

    // Manually advance the ImGui layout cursor down past the drawing
    ImGui::Dummy(ImVec2(total_width, height));
}

static std::vector<int> get_all_color_stop_ids(heatmap_t* heatmap) {
    std::vector<int> result;
    for (int i = 0; i < heatmap->heatmap_colormap.color_stop_count; i++) {
        result.push_back(heatmap->heatmap_colormap.color_stops[i].id);
    }
    return result;
}

static void draw_heatmap_color_buttons(heatmap_t *heatmap) {

    std::vector<int> color_stop_ids = get_all_color_stop_ids(heatmap);
    std::sort(color_stop_ids.begin(), color_stop_ids.end());


    for (int i = 0; i < color_stop_ids.size(); i++)
    {
        color_stop_t new_color_stop = *get_color_stop_by_id(&heatmap->heatmap_colormap, color_stop_ids[i]);
        bool needs_update = false;

        ImGui::PushID(i);

        ImVec4 color = ImVec4(
            new_color_stop.color[0] / 255.0f,
            new_color_stop.color[1] / 255.0f,
            new_color_stop.color[2] / 255.0f,
            new_color_stop.color[3] / 255.0f);
        if (ImGui::ColorEdit4("Color Stop Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar)) {
            new_color_stop.color[0] = color.x * 255.0f;
            new_color_stop.color[1] = color.y * 255.0f;
            new_color_stop.color[2] = color.z * 255.0f;
            new_color_stop.color[3] = color.w * 255.0f;
            needs_update = true;
        }
        ImGui::SameLine();

        float stop_point = new_color_stop.stop_point * 100;
        ImGui::PushItemWidth(200);
        if (ImGui::InputFloat("Stop point", &stop_point, 1.0f, 10.0f, "%.1f%%")) {
            if (stop_point > 100) {
                stop_point = 100;
            }
            else if (stop_point < 0){
                stop_point = 0;
            }
            new_color_stop.stop_point = stop_point / 100.0f;
            needs_update = true;
        };

        if (needs_update) {
            update_color_stop_by_id(&heatmap->heatmap_colormap, color_stop_ids[i],
                new_color_stop.color[0],
                new_color_stop.color[1],
                new_color_stop.color[2],
                new_color_stop.color[3],
                new_color_stop.stop_point
            );
        }

        ImGui::SameLine();

        if (ImGui::SmallButton("Add copy")) {
            add_color_stop(&heatmap->heatmap_colormap,
                new_color_stop.color[0],
                new_color_stop.color[1],
                new_color_stop.color[2],
                new_color_stop.color[3],
                new_color_stop.stop_point
            );
        }; ImGui::SameLine();

        bool disable_remove = false;
        if (heatmap->heatmap_colormap.color_stop_count <= 1) {
            disable_remove = true;
            ImGui::BeginDisabled();
        }
        if (ImGui::SmallButton("Remove")) {
            remove_color_stop_by_id(&heatmap->heatmap_colormap, color_stop_ids[i]);
        };
        if (disable_remove) {
            ImGui::EndDisabled();
            disable_remove = false;
        }

        ImGui::PopID();
    }
}


static void draw_heatmap_color_reset(heatmap_t* heatmap) {
    static int item_current = 0;

    // Show the reset options defined with COLOR_SCHEMES
    if (ImGui::BeginCombo("Color scheme", COLOR_SCHEMES[item_current].name))
    {
        for (int i = 0; i < COLOR_SCHEME_COUNT; i++)
        {
            bool selected = (item_current == i);

            if (ImGui::Selectable(COLOR_SCHEMES[i].name, selected))
                item_current = i;

            if (selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    // Reset heatmap colors using selected color scheme
    if (ImGui::Button("Reset to color scheme")) {
        reset_heatmap_colormap_default_values(&heatmap->heatmap_colormap, item_current);
    }
}
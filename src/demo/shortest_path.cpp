#include "shortest_path.h"

#include <iostream>
#include <vector>

#include "demo_utils.h"

namespace Demo {

ShortestPath::
    ShortestPath() { /*  this->canvas = std::make_unique<Canvas>();  */
}

void ShortestPath::Render() {
    static char input[128] =
        "(A,B,2) (A,C,4) (B,C,1) (B,D,4) (B,E,2) (C,E,3) (D,E,3) (D,F,2) "
        "(E,F,2)";
    ImGui::InputText("Input", input, sizeof(input));
    ImGui::SameLine();
    if (ImGui::Button("Start!")) {
        Dijkstra('A', input);
    }
}

ImVector<ImVec2> ShortestPath::Dijkstra(const char start,
                                        const std::string& __input) {
    auto __input_splitted = Demo::Utils::split(__input, ' ');

    // To find max char in graph
    char max_node;
    for (const auto& c : __input) {
        if (c != '(' || c != ')' || c != ',') {
            max_node = c > max_node ? c : max_node;
        }
    }

    // Construct Adjacency Matrix
    using std::vector;
    u_int matrix_size = max_node - 'A' + 1;
    vector<vector<int>> AM(matrix_size, vector(matrix_size, 0));
    for (const auto& node : __input_splitted) {
        int i(node[1] - 'A'), j(node[3] - 'A');
        AM[i][j] = AM[j][i] = node[5] - '0';  // record cost
    }
    for (int i = 0; i < AM.size(); i++) AM[i][i] = 0;

    // Algorithm main
    vector<SPTableItem> sp_table(AM.size(), SPTableItem());
    sp_table[0].cost = 0;

    auto find_min_idx = [&sp_table]() -> int {
        int min = INT32_MAX, min_idx;
        for (int i = 0; i < sp_table.size(); i++) {
            auto& item = sp_table[i];
            if (!item.visited && item.cost < min) {
                min = item.cost;
                min_idx = i;
            }
        }
        return min_idx;
    };

    for (int visited_cnt = 0; visited_cnt < AM.size() - 1; visited_cnt++) {
        int m = find_min_idx();
        sp_table[m].visited = true;

        for (int i = 0; i < sp_table.size(); i++) {
            if (!sp_table[i].visited             && 
                AM[m][i]                         &&
                sp_table[m].cost != 1000         &&
                AM[i][m] + sp_table[m].cost < sp_table[i].cost)
            {
                sp_table[i].cost = AM[i][m] + sp_table[m].cost;
                sp_table[i].prev = m;
            }
        }
    }

    // Print AM
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            std::cout << AM[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "=============================================\n\n\n";

    for (auto& sp : sp_table) {
        std::cout << sp.cost << '\t' << sp.prev << std::endl;
    }

    return {};
}

}  // namespace Demo
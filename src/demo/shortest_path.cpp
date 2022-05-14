#include "shortest_path.h"

#include <iostream>
#include <vector>
#include <sstream>

#include "demo_utils.h"

namespace Demo {

ShortestPath::
    ShortestPath() { /*  this->canvas = std::make_unique<Canvas>();  */
}

void ShortestPath::Render() {
    static char input[128 * 32] =
        "3 2 1\n"
        "0 1 5\n"
        "1 2 7";
    ImGui::InputTextMultiline("Input", input, sizeof(input));
    std::stringstream ss(input);
    int v, e, a;
    static int res(0);
    ss >> v >> e >> a;
    std::string tmp, _input;
    while(std::getline(ss, tmp)) _input += tmp + ' ';

    ImGui::SameLine();
    if (ImGui::Button("Start!")) {
        res = Dijkstra(a, v, _input);
    }
    ImGui::Text("%d", res);
}

int ShortestPath::Dijkstra(int start, int max_node,
                                        std::string __input) {

    // To find max char in graph

    // Construct Adjacency Matrix
    using std::vector;
    u_int matrix_size = max_node;
    vector<vector<int>> AM(matrix_size, vector(matrix_size, 0));
    std::stringstream ss(__input.c_str());
    int a, b, c;

    while(ss >> a >> b >> c) {
        AM[a][b] = AM[b][a] = c;
    }

    for (int i = 0; i < AM.size(); i++) AM[i][i] = 0;

    // Algorithm main
    vector<SPTableItem> sp_table(AM.size(), SPTableItem());
    sp_table[start].cost = 0;

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

    int max = 0;
    for (int i=start; i < sp_table.size(); ++i) {
        std::cout << sp_table[i].cost << std::endl;
        max = std::max(max, sp_table[i].cost);
    }
    std::cout << max << std::endl;

    return max;
    
} 

}  // namespace Demo
#pragma once
#include <memory>
#include <string>
#include <sstream>

#include "canvas.h"

namespace Demo {

class ShortestPath {
   public:
    ShortestPath();

    void Render();

   // Algorithm's methods
   public:
    int Dijkstra(int start,int  v, std::string __input);
   private:

   private:
    // std::unique_ptr<Canvas> canvas;
};

struct SPTableItem {
    bool visited;
    int  cost;
    int prev;
    SPTableItem() : visited(false), cost(1000), prev(-1) {}
};

}  // namespace Demo
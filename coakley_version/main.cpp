#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "prim.hpp"

int main(int argc, char* argv[]) {
  // parse the expected input
  // first, the number of nodes for the complete graph part
  int num_nodes;
  std::cin >> num_nodes;

  // second, read in the complete graph
  std::vector< std::vector<int> > graph;
  for(int i=0; i<num_nodes; ++i) {
    std::vector<int> row;
    for (int j=0; j<num_nodes; ++j) {
      int cost;
      std::cin >> cost;
      row.push_back(cost);
    }
    graph.push_back(row);
  }

  // calculate the mst (print halfway point)
  Prim prim(graph);
  prim.mst((graph.size()-1) / 2);

  // last bit until newline from last cin followed by a blank line
  std::cin.ignore();
  std::cin.ignore();

  // read in the new edges
  // parse input lines until I find newline
  for(std::string line; std::getline(std::cin, line) && line.compare(""); ) {
    std::stringstream ss(line);
    int from;
    int to;
    int cost;
    ss >> from;
    ss >> to;
    ss >> cost;

    graph[to][from] = graph[from][to] = cost;
    // tell our modified prim's to update auxiliary data structures for the edge
    prim.augment(from, to);
    prim.augment(to, from);
  }

  // finish the mst
  prim.mst(graph.size()-1);

  return 0;
}

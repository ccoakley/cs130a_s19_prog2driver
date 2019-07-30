#include <algorithm>
#include <vector>
#include <limits>
#include <utility>
#include <iostream>

// though it doesn't look like it, this is based on the code from
// https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/

class Prim {
private:
  // the graph, an adjacency matrix
  const std::vector< std::vector<int> > &graph;
  // this will hold a list of edges forming the mst
  // the order is the order Prim's algorithm joins nodes
  std::vector< std::pair<int,int> > tree;
  // distance from a node to the mst, and the node it is closest to
  std::vector< std::pair<int,int> > distance_and_node;
  // what is currently in the mst
  std::vector<bool> in_mst;

public:
  Prim(const std::vector< std::vector<int> > &graph) : graph(graph), in_mst(graph.size(), false) {
    // initialize our vector of best known distance to our current MST
    for(auto it: graph[0]) {
      distance_and_node.push_back(std::make_pair(it, 0));
    }

    // initially, just node 0
    in_mst[0] = true;
  }

  // see if we need to update distance_and_node when changing costs
  void augment(const int from, const int to) {
    // we don't need to update if to is in the mst already or if from isn't
    // i.e. this can't be a candidate edge
    if(in_mst[to] || !in_mst[from]) return;
    // update if this edge is a better candidate than the current candidate for to
    // or (from,to) was the candidate and we just made it worse
    if(graph[from][to] < distance_and_node[to].first) {
      // this is better than the candidate (the edge vertices may be the same, but the cost changed)
      distance_and_node[to] = std::make_pair(graph[from][to], from);
    } else if (distance_and_node[to].second == from && graph[from][to] > distance_and_node[to].first) {
      // potentially find a new candidate, we may have broken this one
      int min = graph[distance_and_node[to].second][to];
      int mindex = distance_and_node[to].second;
      for(unsigned int i=0; i < graph[to].size(); ++i) {
        if (in_mst[i] && graph[i][to] < min) {
          min = graph[i][to];
          mindex = i;
        }
      }
      distance_and_node[to] = std::make_pair(min, mindex);
    }
  }

  // find the next node that is closest to the mst and the node in the mst it is closest to
  int find_closest() const {
    int dist = std::numeric_limits<int>::max();
    int index;

    for(unsigned int i=0; i<distance_and_node.size(); ++i) {
      if(!in_mst[i] && distance_and_node[i].first < dist) {
        dist = distance_and_node[i].first;
        index = i;
      }
    }
    return index;
  }

  // convenience function to print as the assignment requires
  void print_tree() const{
    for (auto it : tree) {
      std::cout << it.first << " " << it.second << std::endl;
    }
    std::cout << std::endl;
  }

  void mst(int stop) {

    // Prim's algorithm
    while (tree.size() < std::min(stop, (int)graph.size()) ) {
      auto closest = find_closest();

      // add closest to the mst
      tree.push_back(std::make_pair(distance_and_node[closest].second, closest));
      in_mst[closest] = true;

      // update the distance vector to see if something is closer to closest
      // than they were to other nodes in the mst already
      for(int i=0; i<graph.size(); ++i) {
        if (graph[closest][i] && !in_mst[i] && graph[closest][i] < distance_and_node[i].first) {
          distance_and_node[i] = std::make_pair(graph[closest][i], closest);
        }
      }
    }

    print_tree();
  }
};

#ifndef GRAPH_ITER
#define GRAPH_ITER

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

/// Iterates over all possible graphs
template <class _Storage>
class Graph_Iterator {
public:
  Graph<_Storage> *g;

  Graph_Iterator(int num_nodes) {
    g = new Graph<_Storage>(num_nodes);
  }

  Graph_Iterator(Graph<_Storage> g_in) {
    g = new Graph<_Storage>(0);
    (*g) = g_in;
  }

  bool next_graph() {
    vector<vector<_Storage> > &m = g->matrix();

    for(int u=0;u<m.size();u++) {
      for(int v=0;v<m[u].size();v++) {
        if(m[u][v] == 0) {m[u][v] = 1; return true;}
          m[u][v] = 0;
      }
    }

    return false;
  }

  int addsafe(int n) {
    vector<vector<bool> > &m = g->matrix();
    
    int sum=0;
    int pos=0;
   
    // if a zero at position larger than int then we can increment
    for(int u=0;u<m.size();u++) {
      for(int v=0;v<m[u].size();v++) {
	pos++;
	if((pos>31) && (m[u][v] == 0)) return n;
      }
    }
    

    // if less than n in int, then we can increment. Otherwise our max is less
    for(int u=0;(u<m.size()) && (pos <= 31);u++) {
      for(int v=0;(v<m[u].size()) && (pos <= 31);v++) {
        if(m[u][v] > 0) sum += (1 << pos);
	pos++;
      }
    }

    if(sum > n) return n;
    if(sum < n) return sum-1;
  }

  bool next_graph(int n) {
    vector<vector<bool> > &m = g->matrix();
    int i_bin = 31;
    for(unsigned int i=0xFFFFFFFF;i>=0;i=i/2,i_bin--) {
//      std::cout << "i: " << i << std::endl;//0
      if((n > i) || (i == 0)) {

        int p = i_bin;
        int u = p/m.size();
        int v = p%m[0].size();
        if(i == 0) {u=0;v=0;n=0;}
      
//      std::cout << "u: " << u << std::endl;//0
//      std::cout << "v: " << v << std::endl;//0
//      std::cout << "n: " << n << std::endl;//1
        n = n - i; 

        bool stop=false;
        for(;(u<m.size()) && (stop != true);u++) {
          for(;(v<m[u].size()) && (stop != true);v++) {
            if(m[u][v] == 0) {m[u][v] = 1; stop=true;} else
            m[u][v] = 0;
          }
          v=0;
        }

        if(i == 0) return true;
      }
    }
  }

  Graph<_Storage> &get_graph() {
    return *g;
  }
};

#endif

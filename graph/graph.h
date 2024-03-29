#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

template <class _Storage>
class Graph {

protected:
  vector<vector <_Storage> > graph_matrix;
  typedef vector<vector <_Storage> > _matrix_type;

public:

  Graph(int num_nodes) {
    graph_matrix.resize(num_nodes);
    for(typename _matrix_type::iterator i = graph_matrix.begin();i != graph_matrix.end();i++) {
      (*i).resize(num_nodes);
    }
    // graph_matrix = new vector<vector<bool> >(num_nodes,vector<bool>(num_nodes,false));
  }
  
  ~Graph() {
  }

  int size() {
    return graph_matrix.size();
  }

  Graph<_Storage> &operator=(Graph<_Storage> &g1) {
    vector<vector <_Storage> > m1 = g1.matrix();
    graph_matrix.clear();

    for(typename _matrix_type::iterator i=m1.begin();i != m1.end();i++) {
      vector<_Storage> line;
      line.insert(line.begin(),(*i).begin(),(*i).end());
      graph_matrix.push_back(line);
    }
  }

  bool operator==(Graph<_Storage> &g1) {
    vector<vector <_Storage> > m1 = g1.matrix();

    for(int x=0;x<graph_matrix.size();x++) {
      for(int y=0;y<graph_matrix[0].size();y++) {
        if(graph_matrix[x][y] != m1[x][y]) return false;
      }
    }

    return true;
  }


  bool add_node() {
    for(typename _matrix_type::iterator i=graph_matrix.begin();i != graph_matrix.end();i++) {
      (*i).push_back(false);
    }

    vector<_Storage> line;
    for(int n=0;n<(*graph_matrix)[0].size();n++) line.push_back(false);
    graph_matrix.push_back(line);
    return true;
  }

  vector<vector<_Storage> > &matrix() {
    return graph_matrix;
  }

  bool get_edge(int n1,int n2) {
    return graph_matrix[n1][n2];
  }
  
  void rand_edge(int n1,int n2) {
    if(n1 < 0) return;
    if(n1 > (graph_matrix)[0].size()) return;
    if(n2 < 0) return;
    if(n2 > (graph_matrix)[0].size()) return;

    graph_matrix[n1][n2] = rand()%2;
  }

  void add_edge(int n1,int n2) {
    (*graph_matrix)[n1][n2] = true;
    (*graph_matrix)[n2][n1] = true;
  }

  void remove_edge(int n1,int n2) {
    (*graph_matrix)[n1][n2] = false;
    (*graph_matrix)[n2][n1] = false;
  }

  void give_edges(int n,bool unidirectional=true,bool noself=true) {
    
    int i=0;
    if(unidirectional) make_unidirectional();
    for(int u=0;u<graph_matrix.size();u++) {
      for(int v=0;v<graph_matrix[u].size();v++) {
        if((graph_matrix[u][v] <= 0) && (i<n)) {
	  if((!noself) || ( u != v)) {
            graph_matrix[u][v] = 1;
	    graph_matrix[v][u] = 1;
	    i++;
	  }
	}
      }
    }
  }

  void randomise() {
    for(int u=0;u<graph_matrix.size();u++) {
      for(int v=0;v<graph_matrix[u].size();v++) {
        graph_matrix[u][v]=rand()%2;
      }
    }
  }

  void randomise_points(int numpoints) {
    for(int i=0;i<numpoints;i++) {
      graph_matrix[rand()%graph_matrix.size()][rand()%graph_matrix.size()] = rand()%2;
    }
  }

  void random_swap(int numpoints) {
    for(int i=0;i<numpoints;i++) {
      random_swap();
    }
  }

  void random_swap(bool unidirectional=true,bool noself=true) {
    int x1 = rand()%graph_matrix.size();
    int x2 = rand()%graph_matrix.size();
    int y1 = rand()%graph_matrix.size();
    int y2 = rand()%graph_matrix.size();
    
    if((!noself) || (x2 != y2)) 
    if((graph_matrix[x1][y1] >= 1) && (graph_matrix[x2][y2] == 0)) {
      int temp = graph_matrix[x1][y1];
      graph_matrix[x1][y1] = graph_matrix[x2][y2];
      graph_matrix[x2][y2] = temp;

      if(unidirectional) {
        int temp2 = graph_matrix[y1][x1];
        graph_matrix[y1][x1] = graph_matrix[y2][x2];
        graph_matrix[y2][x2] = temp2;
        //graph_matrix[y1][x1] = graph_matrix[x1][y1];
	//graph_matrix[y2][x2] = graph_matrix[x2][y1];
      }
    }
  }

  void randomise_points_upto(int n_max,int numpoints) {
    for(int i=0;i<numpoints;i++) {
      graph_matrix[rand()%n_max][rand()%n_max] = rand()%2;
    }
  }

  void randomise_points_upto_outboundallow(int n_max,int numpoints) {
    for(int i=0;i<numpoints;i++) {
      graph_matrix[rand()%n_max][rand()%graph_matrix.size()] = rand()%2;
    }
  }

  void print_matrix_region(int x_max,int y_max) {
    for(int u=0;u<x_max;u++) {
      for(int v=0;v<y_max;v++) {
          cout << graph_matrix[u][v] << " ";
      }
      cout << endl;
    }
  }


  void print_matrix(std::ostream& out = std::cout) const {
    for(int u=0;u<graph_matrix.size();u++) {
      for(int v=0;v<graph_matrix[u].size();v++) {
          out << graph_matrix[u][v] << " ";
      }
      out << endl;
    }
  }

  int neighbours_count(int n) {
    int c=0;
    for(int i=0;i<graph_matrix.size();i++) {
      if((*graph_matrix)[n][i] == true) c++;
    }

    return c;
  }

  int overlap(int n1,int n2,int d) {
    vector<int> n1_neigh = neighbours_within_dist_vec(n1,d);
    vector<int> n2_neigh = neighbours_within_dist_vec(n2,d);

    int count=0;
    // how many same in n1 and n2?
    for(vector<int>::iterator n1_i=n1_neigh.begin();n1_i != n1_neigh.end();n1_i++) {
      int match = false;
      for(vector<int>::iterator n2_i=n2_neigh.begin();n2_i != n2_neigh.end();n2_i++) {
        if((*n1_i) == (*n2_i)) match = true;
      }
      if(match==true) count++;
    }
    return count;
  }

  vector<int> neighbours_vec(int n) {
    vector<int> neighbours;

    for(int i=0;i<graph_matrix.size();i++) {
      // if(graph_matrix[n][i] == true) cout << "neighbours of: " << n << " are " << i << endl;
      if(graph_matrix[n][i] == true) neighbours.push_back(i);
    }

    return neighbours;
  }

  vector<int> neighbours_at_dist_vec(int n,int d) {
    vector<int> neighbours = neighbours_vec(n);

    for(int i=1;i<d;i++) {
      vector<int> new_neighbours;
      for(vector<int>::iterator n_i = neighbours.begin();n_i != neighbours.end();n_i++) {
        
        vector<int> new_neigh = neighbours_vec((*n_i));
        new_neighbours.insert(new_neighbours.end(),new_neigh.begin(),new_neigh.end());
      }
      neighbours.clear();
      neighbours.insert(neighbours.end(),new_neighbours.begin(),new_neighbours.end());
    }

    vector<int> nret;
    for(int i=0;i<graph_matrix.size();i++) {
      int contains=false;
      for(vector<int>::iterator n_i = neighbours.begin();n_i != neighbours.end();n_i++) {
        if((*n_i) == i) contains=true;
      }
      if(contains==true) {
        nret.push_back(i);
      } 
    }

    return nret;
  }



  vector<int> neighbours_within_dist_vec(int n,int d) {
    vector<int> neighbours = neighbours_vec(n);

    for(int i=1;i<d;i++) {
      vector<int> new_neighbours;
      for(vector<int>::iterator n_i = neighbours.begin();n_i != neighbours.end();n_i++) {
        
        vector<int> new_neigh = neighbours_vec((*n_i));
        new_neighbours.insert(new_neighbours.end(),new_neigh.begin(),new_neigh.end());
      }

      neighbours.insert(neighbours.end(),new_neighbours.begin(),new_neighbours.end());
    }

    vector<int> nret;
    for(int i=0;i<graph_matrix.size();i++) {
      int contains=false;
      for(vector<int>::iterator n_i = neighbours.begin();n_i != neighbours.end();n_i++) {
        if((*n_i) == i) contains=true;
      }
      if(contains==true) {
        nret.push_back(i);
      } 
    }

    return nret;
  }


  int neighbours_within_dist_count(int n,int d) {
    vector<int> neighbours = neighbours_vec(n);

    for(int i=1;i<d;i++) {
      vector<int> new_neighbours;
      for(vector<int>::iterator n_i = neighbours.begin();n_i != neighbours.end();n_i++) {
        
        vector<int> new_neigh = neighbours_vec((*n_i));
        new_neighbours.insert(new_neighbours.end(),new_neigh.begin(),new_neigh.end());
      }

      neighbours.insert(neighbours.end(),new_neighbours.begin(),new_neighbours.end());
    }

    // count nodes
    int count=0;
    for(int i=0;i<graph_matrix.size();i++) {
      int contains=false;
      for(vector<int>::iterator n_i = neighbours.begin();n_i != neighbours.end();n_i++) {
        if((*n_i) == i) contains=true;
      }
      if(contains==true) count++;
    }

    return count;
  }

  void make_unidirectional() {
    int c_min=0;
    for(int x = c_min;x < graph_matrix.size();x++) {
      for(int y=0;y < graph_matrix.size();y++) {
        graph_matrix[y][x]=graph_matrix[x][y];
      }
      c_min++;
    }
  }

  void remove_selfedge() {
    for(int x = 0;x < graph_matrix.size();x++) {
      graph_matrix[x][x] = 0;
    }
  }

  template<class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & graph_matrix;
  }

};

template <class _prec>
inline std::ostream& operator<<(std::ostream& out, const Graph<_prec> &rhs) {
  rhs.print_matrix(out);

  return out;
}

#endif

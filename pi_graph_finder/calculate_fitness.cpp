#include "../graph/graph.h"
#include "../graph/graph_iterator.h"
#include "graph_fitness_square.h"
#include "stringify.h"
#include <iostream>
#include <fstream>
#include <vector>

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace mpi = boost::mpi;

using namespace std;

int main(int argc,char *argv[]) {
  
  if(argc < 2) {
    cout << "calculate_fitness <graph file>" << endl;
    return 0;
  }

  // create and open an archive for input
  std::ifstream graph_file(argv[1], std::ios::binary);
  boost::archive::text_iarchive ia(graph_file);
  
  // read class state from archive
  Graph<bool> g(0);
  ia >> g;
  graph_file.close();

  cout << "Graph: " << endl;
  g.print_matrix();


  int score = calculate_fitness(g,1);

  cout << "Graph score: " << score << endl;

  for(int n=0;n<g.size();n++) {
    cout << "Node radiate score: " << calculate_fitness_radiate_node(g,n,1) << endl;
    cout << "Node overlap score: " << calculate_fitness_overlap_node(g,n,1) << endl;
  }

  return 0;
}

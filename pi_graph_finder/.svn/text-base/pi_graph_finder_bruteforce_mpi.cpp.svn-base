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
#include <boost/mpi.hpp>

namespace mpi = boost::mpi;

#define PI 3.14159265

using namespace std;

int main(int argc,char *argv[]) {
  
  // Initialise MPI
  mpi::environment env(argc, argv); 
  mpi::communicator world; 

  if(argc < 3) {
    cout << "pi_graph_finder <size> <filename>" << endl;
    return 0;
  }


  int size=convertTo<int>(argv[1]);
  const char* filename = argv[2];
  
  int rand_rate=2;
  int perpass = 1000;
  int fitness_distance = 1;

  srand(time(NULL));
  Graph<bool> g_c(size);

  cout << "Process started" << endl;  
  if(world.rank() == 0) {
    cout << "Master node" << endl;

    vector<Graph<bool> > graphs;

    Graph_Iterator<bool> gi(size);
    
    Graph<bool> g(0);
    g = *(gi.g);
    graphs.push_back(g);
    for(int i=0;i<(world.size()-1);i++) {
      gi.next_graph(10000);
      
      Graph<bool> g(0);
      g = *(gi.g);
      graphs.push_back(g);
    }

    // Main loop
    bool runcomplete = false;
    Graph<bool> best_graph(size);
    int piscore1 = calculate_fitness(best_graph,fitness_distance);
    int best_piscore = piscore1;

    for(int n=0;!runcomplete;n++) {
      // send graphs out to nodes
      for(int na = 1;na < world.size();na++) {
        // send graphs
        world.send(na, 0, graphs[na]);

	cout << "Sent to " << na << " graph: " << endl;
	graphs[na].print_matrix();
      }

      // receive graphs
      for(int na = 1;na < world.size();na++) {
        // receive graph
        Graph<bool> rgraph(0);
        world.recv(na, 0, rgraph);

        graphs[na] = rgraph;
      
        // score graph
        int piscore = calculate_fitness(graphs[na],fitness_distance);
        if(best_piscore >= piscore) { best_graph = graphs[na]; best_piscore = piscore;}
        
	cout << "Received graph: " << endl;
	rgraph.print_matrix();
	cout << endl;
	cout << "Score: " << piscore << endl;
      
      }

      graphs.clear();

      Graph<bool> g(0);
      g = *(gi.g);
      graphs.push_back(g);
      for(int i=0;i<(world.size()-1);i++) {
        int num = gi.addsafe(10000);
        cout << "num: " << num << endl;
	gi.next_graph(num);

        Graph<bool> g(0);
        g = *(gi.g);
        graphs.push_back(g);
      }

      if(gi.addsafe(10000) == 0) runcomplete=true;
      cout << "Master pass complete" << endl;
      cout << "Current best:" << endl;
      best_graph.print_matrix();

      cout << endl;
      cout << "current score: " << calculate_fitness(best_graph,fitness_distance) << endl;

    }

    cout << "Final graph:" << endl;
    best_graph.print_matrix();

    cout << endl;
    cout << "Final score: " << calculate_fitness(best_graph,fitness_distance) << endl;
    
    
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    const Graph<bool> &gout = best_graph;
    oa << gout;

    for(int i=0;i<graphs[0].size();i++) {
      cout << "Node " << i << " radiate score: " << calculate_fitness_radiate_node(graphs[0],i,fitness_distance) << endl;
      cout << "Node " << i << " overlap score: " << calculate_fitness_overlap_node(graphs[0],i,fitness_distance) << endl;
    }
  }

  if(world.rank() != 0) {
    cout << "Node " << world.rank() << endl;

    Graph<bool> mgraph(0);
    Graph<bool> rbest_graph(0);

    int best_fitness = calculate_fitness(mgraph,fitness_distance);
    for(;;) {
    
      world.recv(0, 0, mgraph);
      Graph_Iterator<bool> gi1(mgraph);
      // Received graph
      //cout << "Received graph: " << endl;
      //mgraph.print_matrix();

      // Climb
      for(int ra=0;ra<(10000-1);ra++) {
	gi1.next_graph();
       
        
	int new_fitness = calculate_fitness(*(gi1.g),fitness_distance);
        
	if((new_fitness > best_fitness) || (ra == 0)) {best_fitness = new_fitness;
                                                       rbest_graph = *(gi1.g);}
      }
      world.send(0, 0, rbest_graph);
      cout << "compute, pass complete" << endl;
    }
  }
}

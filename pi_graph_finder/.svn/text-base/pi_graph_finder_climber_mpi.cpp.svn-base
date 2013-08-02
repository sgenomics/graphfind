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

  if(argc < 4) {
    cout << "pi_graph_finder <size> <passes> <filename>" << endl;
    return 0;
  }


  int size=convertTo<int>(argv[1]);
  int runtime = convertTo<int>(argv[2]);
  const char* filename = argv[3];
  
  int rand_rate=2;
  int perpass = 1000;
  int fitness_distance = 1;

  srand(time(NULL));
  Graph<bool> g_c(size);

  cout << "Process started" << endl;  
  if(world.rank() == 0) {
    cout << "Master node" << endl;

    int best_piscore = 0;
    int best_pos = 0;
    vector<Graph<bool> > graphs;

    for(int i=0;i<world.size();i++) {
      Graph<bool> g(size);
      //g.randomise();
      g.give_edges(2*size);
      g.random_swap(1000);
      //g.make_unidirectional();
      //g.remove_selfedge();
      graphs.push_back(g);
    }

    // Main loop
    for(int n=0;n<runtime;n++) {
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
        if(na == 0) { best_piscore = piscore; best_pos = 0; } 
        if(best_piscore >= piscore) { best_piscore = piscore; best_pos = na;}
      }

      // replicate best graph
      for(int na=0;na < graphs.size();na++) {
        graphs[na] = graphs[best_pos];
      }

      cout << "Master pass complete" << endl;
    }

    cout << "Final graph:" << endl;
    graphs[0].print_matrix();

    cout << endl;
    cout << "Final score: " << calculate_fitness(graphs[0],fitness_distance) << endl;
    
    
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    const Graph<bool> &g = graphs[0];
    oa << g;

    for(int i=0;i<graphs[0].size();i++) {
      cout << "Node " << i << " radiate score: " << calculate_fitness_radiate_node(graphs[0],i,fitness_distance) << endl;
      cout << "Node " << i << " overlap score: " << calculate_fitness_overlap_node(graphs[0],i,fitness_distance) << endl;
    }
  }

  if(world.rank() != 0) {
    cout << "Node " << world.rank() << endl;

    Graph<bool> mgraph(0);

    int best_fitness = calculate_fitness(mgraph,fitness_distance);
    for(int r=0;r<runtime;r++) {
    
      world.recv(0, 0, mgraph);
      // Received graph
      cout << "Received graph: " << endl;
      mgraph.print_matrix();

      // Climb
      for(int ra=0;ra<perpass;ra++) {
        Graph<bool> mgraph_new(mgraph);
        //if(rand()%50 == 42) 
	//mgraph_new.randomise_points(1);
        mgraph_new.random_swap(2);
	
	//mgraph_new.make_unidirectional();
        //mgraph_new.remove_selfedge();

        //cout << "mgraph_new: ";
        //mgraph_new.print_matrix();

        int new_fitness = calculate_fitness(mgraph_new,fitness_distance);
        if(new_fitness > best_fitness) {best_fitness = new_fitness;
                                        mgraph = mgraph_new;}
      }
      world.send(0, 0, mgraph);
      cout << "compute, pass complete" << endl;
    }
  }
}

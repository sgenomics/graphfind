#include "../graph/graph.h"
#include "../graph/graph_iterator.h"
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

using namespace std;
int calculate_fitness(Graph<bool> &g_n,int dist);
int calculate_fitness_node(Graph<bool> &g_n,int node, int dist); 
int calculate_fitness_radiate_node(Graph<bool> &g,int node,int dist);
int calculate_fitness_overlap_node(Graph<bool> &g,int node,int dist);

int calculate_fitness(Graph<bool> &g_n,int dist) {
    int score=0;
    for(int n=0;n<g_n.size();n++) {
      score += calculate_fitness_node(g_n,n,dist);
    }

    return score;
}

int calculate_fitness_node(Graph<bool> &g,int node, int dist) {

  return calculate_fitness_radiate_node(g,node,dist) +
         calculate_fitness_overlap_node(g,node,dist);
}


int calculate_fitness_radiate_node(Graph<bool> &g,int node,int dist) {

  int score_radiate = 0;

  for(int r=1;r<=dist;r++) {
    int c = g.neighbours_within_dist_count(node,r);
    int area = (2*r)*(2*r);
    score_radiate += abs(area-c);
  }

  return score_radiate;
}

int calculate_fitness_overlap_node(Graph<bool> &g,int node,int dist) {
  int score_overlap = 0;
  int r=1;
//  for(int r=1;r<=dist;r++) {
    // check if all neighbours areas overlap we this nodes area
    // at this distance correctly

    int d=r;
    float lens = ((2*r)*(2*r)) - ((2*r)+((2*r)-1))-1; // we don't include ourself in overlap?

    vector<int> neighbours = g.neighbours_vec(node);
    for(vector<int>::iterator i=neighbours.begin();i != neighbours.end();i++) {
      int overlap = g.overlap(node,(*i),r);
      //cout << (*i) << " overlap: " << overlap << endl;
      score_overlap += abs(lens-overlap);
    }
//  }

  return score_overlap;
}

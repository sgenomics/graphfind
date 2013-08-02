#include "../graph/graph.h"
#include "../graph/graph_iterator.h"
#include <iostream>
#include <math.h>

#define PI 3.141

using namespace std;

int main(int argc,char **argv) {

  if(argc < 4) {
    cout << "pi_graph_finder <size> <dist> <printevery>" << endl;
    return 0;
  }

  int size=atoi(argv[1]);
  int dist=atoi(argv[2]);
  int printevery=atoi(argv[3]);

  int best=INT_MAX;
  graph best_graph(0);

  for(int n=0;;n++) {
    graph g(size);
    g.randomise();

    // check graph

    // check if all nodes radiate as pi*r^2

    // count neighbours
    int good=0;

    for(int n_a=0;n_a<size;n_a++) {
      for(int d=1;d<=dist;d++) {
        int c = g.neighbours_within_dist_count(n_a,d);
  
        int area = round(PI*(d*d));
        good += abs(area-c);
      }
    }

    if(good <= best) {
      best=good;
      best_graph = g;
    }

    if(n%printevery == 0) {
      cout << "best match so far: " << best << endl;
      best_graph.print_matrix();
    }
  }

  cout << "best match was: " << best << endl;
  best_graph.print_matrix();
}

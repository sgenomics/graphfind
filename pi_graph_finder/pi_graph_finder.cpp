#include "../graph/graph.h"
#include "../graph/graph_iterator.h"
#include <iostream>
#include <math.h>

#define PI 3.141

using namespace std;

int main(int argc,char **argv) {

  if(argc < 3) {
    cout << "pi_graph_finder <size> <dist>" << endl;
    return 0;
  }

  int size=atoi(argv[1]);
  int dist=atoi(argv[2]);

  Graph_Iterator<int> gi(size*size);

  int best=0x0;
  Graph<int> best_graph(size*size);

  //for(int n=0;gi.next_graph();n++) {
  for(int n=0;;n++) {
    Graph<int> g = best_graph;//gi.get_graph();
//    g.randomise_points(5);

    for(int r=0;r<3;r++) {
      int n = rand()%(size*size);
      int i = rand()%8;
      if(i==0) g.rand_edge(n,n-size-1);
      if(i==1) g.rand_edge(n,n-size);
      if(i==2) g.rand_edge(n,n-size+1);
      if(i==3) g.rand_edge(n,n-1);
      if(i==4) g.rand_edge(n,n+1);
      if(i==5) g.rand_edge(n,n+size);
      if(i==6) g.rand_edge(n,n+size-1);
      if(i==7) g.rand_edge(n,n+size+1);
    }
    // check graph

    // check if all nodes radiate as pi*(r^2)

    // count neighbours
    int good=0;

    for(int n_a=size*3;n_a<((size*size)-size*3);n_a++) {
      for(int d=1;d<=dist;d++) {
        int c = g.neighbours_at_dist_vec(n_a,d).size();

        if(c==d) good++;
  
      }
    }

    if(good >= best) {
      best_graph = g;
      if(good > best) {
        cout << "best match so far: " << best << endl;
        best_graph.print_matrix();
      }
      best=good;
    }

    if(n%100000 == 0) {
      cout << "best match so far: " << best << endl;
      best_graph.print_matrix();
    }
  }

  cout << "best match was: " << best << endl;
  best_graph.print_matrix();
}

#include "../graph/graph.h"
#include "../graph/graph_iterator.h"
#include <iostream>
#include <math.h>
#include <vga.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265

using namespace std;

int calculate_fitness(graph &g_n,int dist,int num_dont_use);

int main(int argc,char **argv) {

  if(argc < 6) {
    cout << "pi_graph_finder <size> <num_dont_use> <dist> <rand_rate> <rand_dec_trigger> <ramp after> <ramp to> <printevery>" << endl;
    return 0;
  }

  int size=atoi(argv[1]);
  int numuse=atoi(argv[2]);
  int dist=atoi(argv[3]);
  int start_r_rate=atoi(argv[4]);
  int dec_r_rate_trigger=atoi(argv[5]);
  int ramp_after=atoi(argv[6]);
  int ramp_max=atoi(argv[7]);
  int printevery=atoi(argv[8]);

  int r_rate=start_r_rate;
  int best=INT_MAX;

  srand(time(NULL));
  graph g_c(size);
  int no_movement=0;
  for(int n=0;;n++) {
    graph g_n(0);
    g_n = g_c;
    g_n.randomise_points(r_rate);
    g_n.make_unidirectional();

    int good = calculate_fitness(g_n,dist,numuse);
    if(good <= best) {
      best=good;
      g_c = g_n;
      if(good < best) {
        no_movement=0;
        r_rate=start_r_rate;
      }
    } else no_movement++;

    if((dec_r_rate_trigger != 0) && (no_movement>=dec_r_rate_trigger)) {
      r_rate--;
      if(r_rate < 1) r_rate=start_r_rate;
      cout << "randomisation rate: " << r_rate << endl;
      no_movement=0;
    }

//    cout << "good_overlap: " << good_overlap << endl;
//    cout << "good radiate: " << good_radiate << endl;
//    cout << "best match so far: " << best << endl;
    if(n%printevery == 0) {
      cout << "best match so far: " << best << endl;
      cout << "generations: " << n << endl;
      g_c.print_matrix();
    }

    // ramp size up slowly
    if(n%ramp_after == 0) {
      if(size < ramp_max) {
        g_c.add_node();
        best = calculate_fitness(g_c,dist,numuse);
        size++;
      }
    }
  }

  cout << "best match was: " << best << endl;
  g_c.print_matrix();
}

int calculate_fitness(graph &g_n,int dist,int num_dont_use) {
    int good_radiate=0;
    int good_overlap=0;

    // radiation
    for(int n_a=0;n_a<(g_n.size()-num_dont_use);n_a++) {
      for(int r=1;r<=dist;r++) {
        int c = g_n.neighbours_within_dist_count(n_a,r);
  //      int area=(r+1)*(r+1);
        int area = round(PI*((r+1)*(r+1)));
        good_radiate += abs(area-c);
      }
    }

/*    for(int n_a=0;n_a<(g_n.size()-num_dont_use);n_a++) {
      for(int r=1;r<=dist;r++) {

        // check if all neighbours areas overlap we this nodes area
        // at this distance correctly

        int d=r;
        float lens = (2*(float)(r*r)*acos((float)d/(2*(float)r)))-(0.5*(float)d*sqrt((float)(4*r*r)-(float)(d*d)));

        int overlap_req = lens;
     
        vector<int> neighbours = g_n.neighbours_at_dist_vec(n_a,r);
        for(vector<int>::iterator i=neighbours.begin();i != neighbours.end();i++) {
          if(((*i) < (g_n.size()-num_dont_use)) && ((*i) != n_a)) {
            int overlap = g_n.overlap(n_a,(*i),r);
            good_overlap += abs(overlap_req-overlap);
          }
        }
      }
    }*/

    return good_radiate+good_overlap;
}

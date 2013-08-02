//#define _POSIX_C_SOURCE 199309L
//#define _BSD_SOURCE

#include <string.h>
#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>

#include <limits.h>

#include <time.h>
#include <stdbool.h>
#include "../graph/graph.h"
#include "../graph/graph_iterator.h"
#include <iostream>
#include <math.h>

#define PI 3.141


void redraw_required();

int font_width  = 8;
int font_height = 16;
int font_space  = 0;

bool any_blinking       = false;
bool new_screen_size    = false;
int  new_screen_size_x;
int  new_screen_size_y;

static int cols;
static int rows;

int display_width;
int display_height;
int display_width_last_kb=0;
int display_height_last_kb=0;
int display_width_abs;
int display_height_abs;

struct SDL_Window   *screen  =(SDL_Window   *) 1;
struct SDL_Renderer *renderer=(SDL_Renderer *) 1;

void do_sdl_init() {
    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        return;
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE  , 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE , 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    screen=SDL_CreateWindow("grender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
 
    if (screen == 0) {
      printf("Could not initialize Window\n");
      printf("Error: %s\n",SDL_GetError());
    }
    
    renderer = SDL_CreateRenderer(screen, -1, NULL);
    if(renderer == 0) {
      printf("Error, bad renderer\n");
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,0xff,0xff,0xff,0xff);
    SDL_RenderClear(renderer);
}


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
  do_sdl_init();

  for(int n=0;;n++) {
    Graph<int> g = best_graph;//gi.get_graph();
//    g.randomise_points(5);

    for(int r=0;r<10;r++) {
      int n = rand()%(size*size);
      int i = rand()%8;
      if((n%size) < 1) break;
      if((n%size) == (size-1)) break;
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

    for(int n_a=size;n_a<((size*size)-size);n_a++) {
      for(int d=1;d<=dist;d++) {
        int c = g.neighbours_at_dist_vec(n_a,d).size();

        if(c==(d+1)) good++;
  
      }
    }

    if(good >= best) {
      best_graph = g;
        cout << "best match so far: " << best << endl;
     //   best_graph.print_matrix();
    SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xff);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);

    for(int i=0;i<(size*size);i++) {
      for(int j=0;j<(size*size);j++) {
        if(best_graph.get_edge(i,j)) {

          int ix = i%size;
          int iy = i/size;
          int jx = j%size;
          int jy = j/size;

          SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
          SDL_RenderDrawLine(renderer,ix*10,iy*10,jx*10,jy*10);
          int xd = (jx*10)-(ix*10);
          cout << (xd/3)*2 << endl;
          int yd = (jy*10)-(iy*10);
          SDL_SetRenderDrawColor(renderer,0xFF,0x0F,0x0F,0xFF);
          SDL_RenderDrawLine(renderer,(ix*10)+((xd/6)*5),(iy*10)+((yd/6)*5),jx*10,jy*10);
        }
      }
    }
  
    SDL_RenderPresent(renderer);
     
      best=good;
    }


    if(n%100000 == 99999) {
      cout << "best match so far: " << best << endl;
      best_graph.print_matrix();
    }
  }

  cout << "best match was: " << best << endl;
  best_graph.print_matrix();
}

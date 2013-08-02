#include "utf.h"
#include "../graph.h"
#include "../graph_iterator.h"
#include <iostream>

void test_graph_iterator(UnitTest &utf) {
  
  Graph_Iterator<bool> gi1(16);
  Graph_Iterator<bool> gi2(16);

  for(int n=0;n<1000;n++) {
    gi1.next_graph();
  }

  gi2.next_graph(1000);

  utf.utf_test(*(gi1.g),*(gi2.g));

  Graph_Iterator<bool> gi3(16);
  Graph_Iterator<bool> gi4(16);
  
  for(int n=0;n<5000;n++) {
    gi3.next_graph();
    gi4.next_graph(1);
  
    utf.utf_test(*(gi3.g),*(gi4.g));
  }
};

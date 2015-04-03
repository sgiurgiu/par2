#include <stdio.h>
#include "par2.h"

int main(int argc, char **argv) {
  enum Par2Result result =  verify("test_data/test.par2");
  printf("\ndone %d\n",result);
  return 0;
}

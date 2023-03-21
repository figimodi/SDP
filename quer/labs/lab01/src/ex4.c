#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
  int i;
  for (i=0; i<2; i++) {
    fprintf (stdout, "i=%d\n", i);
    if (fork())             // Call # 1
      fork ();              // Call # 2
    else
      system ("echo i=i");  // Call # 3
  }
  return (1);
}

/*
  i = 0 x1
  i = 1 x3
  i = i x4
*/
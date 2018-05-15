//a little fun brought to you by bla

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h>

void catcher()
{
  printf("WIN!\n");
  system("/bin/sh");
  exit(0);
}

int main(int argc, char **argv)
{
	puts("source code is available in level02.c\n");

  if (argc != 3 || !atoi(argv[2]))
    return 1;
  signal(SIGFPE, catcher);

  int arg1 = atoi(argv[1]);
  int arg2 = atoi(argv[2]);
  int int_min = INT_MIN;
  printf("abs(arg1): %d\n", abs(arg1));
  printf("arg2: %d\n", arg2);
  printf("int_min: %d\n", int_min);
  printf("result = %d\n", abs(arg1) / arg2);
  return abs(atoi(argv[1])) / atoi(argv[2]);
}


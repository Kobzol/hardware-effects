#include <cpuid.h>
#include <stdio.h>

int main() {
  unsigned long a, b, c, d;
  __cpuid(2, a, b, c, d);
  printf("%lx %lx %lx %lx\n", a, b, c, d);
  return 0;
}

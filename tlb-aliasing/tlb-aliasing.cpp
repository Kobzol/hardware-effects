#include <asm/unistd.h>
#include <errno.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#define Kibibytes(x) (x * 1024ull)
#define Mebibytes(x) (Kibibytes(x) * 1024)
#define Gibibytes(x) (Mebibytes(x) * 1024)
#define Tebibytes(x) (Gibibytes(x) * 1024)

#define REPETITIONS 100000

int main(int argc, char** argv) {
  if (argc < 3) {
    dprintf(STDERR_FILENO, "tlb-aliasing <count> <page_stride>\n");
    return 1;
  }
  int count = atoi(argv[1]);
  int page_stride = atoi(argv[2]);
  u32 block_size = getpagesize() * page_stride;
  long line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  if (line_size <= 0) {
    line_size = 64;
  }
  void* mem =
      mmap((void*)Tebibytes(2ul), block_size * count, PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (mem == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  struct perf_event_attr peattr = {};
  peattr.type = PERF_TYPE_HW_CACHE;
  peattr.size = sizeof(peattr);
  peattr.config = PERF_COUNT_HW_CACHE_DTLB |
                  (PERF_COUNT_HW_CACHE_OP_READ << 8) |
                  (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
  peattr.disabled = 1;
  peattr.exclude_kernel = 1;
  int perf_fd = syscall(__NR_perf_event_open, &peattr, 0, -1, -1, 0);
  if (perf_fd == -1) {
    perror("perf_event_open");
    return 1;
  }

  u8** v = (u8**)malloc(sizeof(*v) * count);

  auto* x = (u8*)mem;
  // NOTE(btolsch): This will hit a new TLB entry on every access because we
  // jump by at least a page, but hopefully there are no cache misses after the
  // first iteration because we also offset by one cache line each time.
  for (int i = 0; i < count; ++i) {
    v[i] = x + (i * block_size + ((i * line_size) % block_size));
  }
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);
  ioctl(perf_fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);
  for (int i = 0; i < REPETITIONS; ++i) {
    for (int j = 0; j < count; ++j) {
      *v[j] += i;
    }
  }
  ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);

  u64 misses = 0;
  int result = read(perf_fd, &misses, sizeof(misses));
  if (result == -1) {
    perror("read perf_fd");
    return 1;
  }
  printf("%.02f misses per repetition (%lu total)\n",
         (float)misses / REPETITIONS, misses);
  printf("%lu us\n", (end.tv_sec * 1000000 + end.tv_nsec / 1000) -
                         (start.tv_sec * 1000000 + start.tv_nsec / 1000));

  return 0;
}

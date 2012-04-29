#if defined(LINUX)
#include "user/util.h"
#include "types.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#define O_CREATE O_CREAT
#define xfork() fork()
#define xexit() exit(EXIT_SUCCESS)
#define xcreat(name) open((name), O_CREATE|O_WRONLY, S_IRUSR|S_IWUSR)
#define mtenable(x) do { } while(0)
#define mtdisable(x) do { } while(0)
#else
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "amd64.h"
#include "mtrace.h"
#define xfork() fork(0)
#define xexit() exit()
#define xcreat(name) open((name), O_CREATE|O_WRONLY)
#endif

#define CHUNKSZ 512
#define FILESZ  (NCPU*CHUNKSZ)

static const bool pinit = true;

static char chunkbuf[CHUNKSZ];

static void
bench(int tid, int nloop)
{
  if (pinit)
    setaffinity(tid);

  int fd = open("filebenchx", O_RDWR);
  if (fd < 0)
    die("open");

  for (int i = 0; i < nloop; i++) {
    ssize_t r;

    r = pread(fd, chunkbuf, CHUNKSZ, CHUNKSZ*tid);
    if (r != CHUNKSZ)
      die("pread");

    r = pwrite(fd, chunkbuf, CHUNKSZ, CHUNKSZ*tid);
    if (r != CHUNKSZ)
      die("pwrite");
  }

  close(fd);
  xexit();
}

int
main(int ac, char **av)
{
  int nthread;
  int nloop;

#ifdef HW_qemu
  nloop = 50;
#else
  nloop = 1000;
#endif

  if (ac < 2)
    die("usage: %s nthreads [nloop]", av[0]);

  nthread = atoi(av[1]);
  if (ac > 2)
    nloop = atoi(av[2]);

  // Setup shared file
  unlink("filebenchx");
  int fd = xcreat("filebenchx");
  if (fd < 0)
    die("open O_CREAT failed");
  for (int i = 0; i < FILESZ; i += CHUNKSZ) {
    int r = write(fd, chunkbuf, CHUNKSZ); 
    if (r < CHUNKSZ)
      die("write");
  }
  close(fd);

  mtenable("xv6-filebench");
  u64 t0 = rdtsc();
  for (int i = 0; i < nthread; i++) {
    int pid = xfork();
    if (pid == 0)
      bench(i, nloop);
    else if (pid < 0)
      die("fork");
  }

  for (int i = 0; i < nthread; i++)
    wait();
  u64 t1 = rdtsc();
  mtdisable("xv6-filebench");

  printf("filebench: %lu\n", t1-t0);
  return 0;
}

/* This tests the key system calls depended on by Charliecloud. */

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#define TRY(x) if (x) { printf("%d: %d\n", __LINE__, errno); exit(1); }

int main(void)
{
   TRY(unshare(CLONE_NEWNS|CLONE_NEWUSER));
   TRY(mount("/tmp", "/", NULL, MS_MOVE, NULL));
   if (mkdir("/newroot", 0755))
      if (errno != EEXIST)
         TRY(errno);
   TRY(mount(NULL, "/newroot", "tmpfs", 0, NULL));
   TRY(mount("/newroot", "/newroot", NULL, MS_REC|MS_BIND|MS_PRIVATE, NULL));
   TRY(mkdir("/newroot/oldroot", 0755));
   TRY(syscall(SYS_pivot_root, "/newroot", "/newroot/oldroot"));

   printf("ok\n");
}

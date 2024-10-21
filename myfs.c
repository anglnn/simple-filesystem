#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static int my_getattr(const char *path, struct stat *stat) {
  /* Debug message */
  printf("[GETATTR] : Get attributes of %s\n", path);

  stat->st_uid = getuid();     /* User ID of owner */
  stat->st_gid = getgid();     /* Group ID of owner */
  stat->st_atime = time(NULL); /* Time of last access */
  stat->st_mtime = time(NULL); /* Time of last modification */

  if (strcmp(path, "/") == 0) {
    stat->st_mode = S_IFDIR | 0755; /* File type and mode */
    stat->st_nlink = 2;             /* Number of hard links */
  } else {
    stat->st_mode = S_IFREG | 0644; /* File type and mode */
    stat->st_nlink = 1;             /* Number of hard links */
    stat->st_size = 1024;           /* Total size, in bytes */
  }

  return 0;
}

static int my_readdir(const char *path, void *buffer, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info *fi) {
  /* Debug message */
  printf("[READDIR] : Get list of files in %s\n", path);

  filler(buffer, ".", NULL, 0);  /* Current Directory */
  filler(buffer, "..", NULL, 0); /* Parent Directory */

  if (strcmp(path, "/") == 0) {
    filler(buffer, "blog_gistre.md", NULL, 0);
    filler(buffer, "yet_another_kind_of_file", NULL, 0);
  }

  return 0;
}

static int my_read(const char *path, char *buffer, size_t size, off_t offset,
                   struct fuse_file_info *fi) {
  char firstFileText[] =
      "You can find lot of intersting articles at : blog.gistre.epita.fr";
  char secondFileText[] = "Hello World from Yet Another Kind of File !";
  char *selectedText = NULL;

  if (strcmp(path, "/blog_gistre.md") == 0)
    selectedText = firstFileText;
  else if (strcmp(path, "/yet_another_kind_of_file") == 0)
    selectedText = secondFileText;
  else
    return -1;

  memcpy(buffer, selectedText + offset, size);

  /* Debug message */
  printf("[READ] : File : %s Content : %s\n", path, selectedText);

  return strlen(selectedText) - offset;
}

static struct fuse_operations myfs_operations = {
    .getattr = my_getattr,
    .readdir = my_readdir,
    .read = my_read,
};

int main(int argc, char *argv[]) {
  return fuse_main(argc, argv, &myfs_operations, NULL);
}

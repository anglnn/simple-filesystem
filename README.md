# simple-filesystem

This is the code used in the article "How to implement our own filesystem" at *blog.gistre.epita.fr*.

You can use GCC to compile it. To see debug messages, use the -f flag. We need to provide the compiler the arguments to include fuse library, to do so we use `pkg-config`.

```sh
$ gcc myfs.c -o myfs `pkg-config fuse --cflags --libs`
$ ./myfs -f [mount point]
```

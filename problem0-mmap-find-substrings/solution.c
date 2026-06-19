#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_string>\n", argv[0]);
        return 1;
    }

    const char *needle = argv[2];
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return 0;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        return 1;
    }

    size_t file_size = (size_t)st.st_size;
    if (file_size < needle_len) {
        close(fd);
        return 0;
    }

    char *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    char first = needle[0];
    char *cur = data;
    char *end = data + file_size - needle_len + 1;
    while (cur < end) {
        cur = memchr(cur, first, (size_t)(end - cur));
        if (cur == NULL) {
            break;
        }
        if (memcmp(cur, needle, needle_len) == 0) {
            printf("%zu\n", (size_t)(cur - data));
        }
        ++cur;
    }

    munmap(data, file_size);
    close(fd);

    return 0;
}

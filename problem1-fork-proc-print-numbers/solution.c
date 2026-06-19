#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    for (int i = 1; i <= n; ++i) {
        printf("%d%c", i, i == n ? '\n' : ' ');
        fflush(stdout);

        if (i == n) {
            break;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid > 0) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                perror("waitpid");
                return 1;
            }
            return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
        }
    }

    return 0;
}

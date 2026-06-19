#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <CMD1> <CMD2>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t first = fork();
    if (first < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    if (first == 0) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            _exit(1);
        }
        close(pipefd[1]);
        execlp(argv[1], argv[1], (char *)NULL);
        perror("execlp");
        _exit(1);
    }

    pid_t second = fork();
    if (second < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(first, NULL, 0);
        return 1;
    }
    if (second == 0) {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("dup2");
            _exit(1);
        }
        close(pipefd[0]);
        execlp(argv[2], argv[2], (char *)NULL);
        perror("execlp");
        _exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    int status_first;
    int status_second;
    if (waitpid(first, &status_first, 0) < 0) {
        perror("waitpid");
        return 1;
    }
    if (waitpid(second, &status_second, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    if (!WIFEXITED(status_first) || WEXITSTATUS(status_first) != 0) {
        return 1;
    }
    if (!WIFEXITED(status_second)) {
        return 1;
    }
    return WEXITSTATUS(status_second);

    return 0;
}

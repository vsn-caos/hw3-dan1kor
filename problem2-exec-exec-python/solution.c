#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {
    char *args[] = {
        "python3",
        "-c",
        "import sys; print(eval(sys.stdin.read()))",
        NULL
    };
    execvp("python3", args);
    return 0;
}
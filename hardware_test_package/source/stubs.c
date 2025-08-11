// 最小的stubs实现，避免链接错误
void _exit(int status) {
    while(1);
}

void _kill(int pid, int sig) {
}

int _getpid(void) {
    return 1;
}
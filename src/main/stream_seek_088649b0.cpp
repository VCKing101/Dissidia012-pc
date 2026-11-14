#include "main/agent2_stream.h"
extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" long long func_089C5EB8(int, long long, int);
extern "C" void func_088649B0(Agent2Stream *self, int fd, int offset,
                              int whence) {
    for (;;) {
        func_08864B68(self, 6);
        int result = (int)func_089C5EB8(fd, offset, whence);
        if (result >= 0) break;
    }
}

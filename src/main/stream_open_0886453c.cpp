#include "main/agent2_stream.h"

extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" int func_089C5EA8(const char *, int, int);

extern "C" int func_0886453C(Agent2Stream *self, const char *path,
                             unsigned char mode) {
    int flags = 1;
    int fd;
    if (mode == 1) flags = 0x40000001;
    for (;;) {
        func_08864B68(self, 1);
        fd = func_089C5EA8(path, flags, 0);
        if (fd >= 0) break;
        if (mode == 1) break;
    }
    return fd;
}

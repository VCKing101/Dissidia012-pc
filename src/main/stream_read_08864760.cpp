#include "main/agent2_stream.h"

extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" int func_089C5E30(int, void *, int);

extern "C" int func_08864760(Agent2Stream *self, int fd, void *buffer,
                             int size, unsigned char mode) {
    int count;
    for (;;) {
        func_08864B68(self, 3);
        count = func_089C5E30(fd, buffer, size);
        if (count >= 0) break;
        if (mode == 1) return 0;
    }
    return count;
}

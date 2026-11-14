#include "main/agent2_stream.h"

extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" int func_089C5E48(int);

extern "C" void func_08864B04(Agent2Stream *self, int fd) {
    for (;;) {
        func_08864B68(self, 8);
        int result = func_089C5E48(fd);
        if (result == 0) break;
    }
}

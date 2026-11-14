#include "main/agent2_stream.h"

typedef struct Agent2IoStat {
    int st_mode;
    unsigned int st_attr;
    long long st_size;
    unsigned char unknown_0010[0x48];
} Agent2IoStat;

extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" int func_089C5E68(const char *, Agent2IoStat *);
extern "C" int func_088642A0(Agent2Stream *self, const char *path,
                             unsigned char mode) {
    for (;;) {
        Agent2IoStat stat;
        func_08864B68(self, 0);
        if (func_089C5E68(path, &stat) == 0) return (int)stat.st_size;
        if (mode == 1) return 0;
    }
}

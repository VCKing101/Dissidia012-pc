#include "main/agent2_stream.h"

extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" void func_089C5E60(const char *, int, const char *);
extern "C" int func_089C5E38(const char *, long long, int);

extern "C" void func_08864A3C(Agent2Stream *self, const char *path,
                              int offset, int length, int first,
                              int second) {
    func_089C5E60(path, self->field_000C, path);
    self->field_0014 = first;
    self->field_0018 = second;
    self->field_0148 = 0;
    self->field_0020 = 2;
    self->field_002C = offset;
    self->field_0030 = length;
    for (;;) {
        func_08864B68(self, 0x87);
        int result = func_089C5E38(path, offset, length);
        if (result == 0) break;
    }
}

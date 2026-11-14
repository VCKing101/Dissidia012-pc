#include "main/agent2_stream.h"

extern "C" void func_08864B68(Agent2Stream *, int);
extern "C" void func_089C5E60(const char *, int, const char *);
extern "C" int func_089C5E90(const char *, unsigned int, void *, int, void *,
                             int);

extern "C" bool func_088648FC(Agent2Stream *self, const char *device,
                              int first, int second) {
    func_089C5E60(device, self->field_0008, device);
    self->field_0014 = first;
    self->field_0018 = second;
    self->field_0148 = 0;
    self->field_0020 = 4;
    for (;;) {
        func_08864B68(self, 0x85);
        int result = func_089C5E90(device, 0x01020007, 0, 0,
                                   &self->field_0040, 8);
        if (result == 0) break;
    }
    return true;
}

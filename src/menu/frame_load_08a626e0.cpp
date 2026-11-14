/* Runs virtual step 8, loads seven frames of the icon and registers them. */
#include "menu/agent12_types.h"

class FrameLoadPart : public A12Part24 {
public:
    char pad_028[0x30 - 0x28];
    void *field_30;
    char pad_034[0xC8 - 0x34];
    int field_C8[7];
};

extern "C" int func_088DAA68(void *icon);
extern "C" void func_088DAB7C(void *icon, int frame);
extern "C" void func_088D3CF0(int zero, int *frames, void *table);
extern "C" char D_08A637C0[];

extern "C" {
void func_08A626E0(FrameLoadPart *self)
{
    int i;
    self->v8();
    for (i = 0; i < 7; i++) {
        int frame = func_088DAA68(self->field_30);
        self->field_C8[i] = frame;
        func_088DAB7C(self->field_30, frame);
    }
    func_088D3CF0(0, self->field_C8, D_08A637C0);
}
}

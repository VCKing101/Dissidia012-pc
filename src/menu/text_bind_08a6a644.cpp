/* Runs virtual step 9, opens the text resource and binds it to the label. */
#include "menu/agent12_types.h"

class TextBindPart : public A12Part24 {
public:
    char pad_028[0x5B8 - 0x28];
    char field_5B8[0x70];
    int field_628;
};

extern "C" int func_0887B3EC(void *source, int kind, int a, int b, int c, int d);
extern "C" void func_088D428C(void *label, int *handle, void *table, int one);
extern "C" char D_08A6B348[];

extern "C" {
void func_08A6A644(TextBindPart *self, void *source, int kind, int id)
{
    self->v9();
    self->field_628 = func_0887B3EC(source, kind, 0, id, 0, 0);
    func_088D428C(self->field_5B8, &self->field_628, D_08A6B348, 1);
}
}

#include "menu/agent29_types.h"

/* MenuRoot: shows help text D_08A23D88[row * 2 + column] (none when 0xFFFF)
   with the root's messages. */
extern "C" {
extern const unsigned short D_08A23D88[];
void func_0888550C(void *object, void *messages, int arg, int id);
}

/* Inlined call on the object in D_089D69E8. */
struct Object9D69E8 {
    static void Show(void *messages, int id) {
        func_0888550C(D_089D69E8, messages, 1, id);
    }
};

extern "C" void func_08A1E1C8(MenuRoot *self, int row, int column)
{
    if (D_08A23D88[row * 2 + column] != 0xFFFF) {
        Object9D69E8::Show((void *)WORK_INT(self->work, 0x88C), D_08A23D88[row * 2 + column]);
    }
}

#include "menu/agent29_types.h"

/* Screen1AB08: once the menu root's lists are ready and the help window
   has closed, moves to state 3. */
extern "C" {
void func_08A1E168(MenuRoot *root);
int func_08A1E1A8(MenuRoot *root);
int func_08A20378(MenuRoot *root);
int func_08885938(void *object);
void func_08885988(void *object);
int func_08885608(void *object, char value);
}

/* Inlined call on the object in D_089D69E8. */
struct Object9D69E8 {
    static int Accept(char value) { return func_08885608(D_089D69E8, value); }
};

extern "C" void func_08A19258(Screen1AB08 *self)
{
    func_08A1E168(D_08A23FF0);
    if (func_08A1E1A8(D_08A23FF0) == 0) {
        return;
    }
    if (func_08A20378(D_08A23FF0) == 0) {
        return;
    }
    if (func_08885938(D_089D69E8) != 0) {
        return;
    }
    func_08885988(D_089D69E8);
    if (Object9D69E8::Accept(1) == 0) {
        return;
    }
    WORK_INT(self->work, 0x10E8) = 3;
}

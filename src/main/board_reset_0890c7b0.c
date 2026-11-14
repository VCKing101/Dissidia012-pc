#include "main/agent21_types.h"

extern void func_088720B4(int mode);
extern void func_088720E4(void);

void func_0890C7B0(Board *board, unsigned char full)
{
    board->field_14 = 0;
    board->field_18 = 0;
    if (full == 1) {
        board->flags.bits.visible = 0;
        board->flags.bits.active = 0;
        func_088720B4(0);
        func_088720E4();
    } else {
        board->flags.bits.active = 0;
    }
}

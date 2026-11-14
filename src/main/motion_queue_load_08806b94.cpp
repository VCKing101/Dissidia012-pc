#include "main/agent30_types.h"

/* Fills the motion queue's slots from the entries of a motion set; each
   entry's flags say which slot it serves. */
struct MotionEntry08806B94 {
    short id;
    unsigned short flags;

    static bool has(MotionEntry08806B94 *e, int bit) { return (e->flags & bit) != 0; }
};

extern "C" {

MotionEntry08806B94 *func_088CB8C8(int set, int *iter);

void func_08806B94(MotionQueue *self, int set)
{
    int iter = 0;
    MotionEntry08806B94 *e;
    while ((e = func_088CB8C8(set, &iter)) != 0) {
        if (MotionEntry08806B94::has(e, 0x4)) {
            self->motions[1] = e;
        }
        if (MotionEntry08806B94::has(e, 0x8)) {
            self->motions[2] = e;
        }
        if (MotionEntry08806B94::has(e, 0x10)) {
            self->motions[3] = e;
        }
        if (MotionEntry08806B94::has(e, 0x20)) {
            self->motions[4] = e;
        }
        if (MotionEntry08806B94::has(e, 0x40)) {
            self->motions[5] = e;
        }
        if (MotionEntry08806B94::has(e, 0x80)) {
            self->motions[6] = e;
        }
    }
}

}

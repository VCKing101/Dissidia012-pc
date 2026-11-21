#include "main/agent33_texture.h"

/* A part's motion channel table entry (0x60 bytes). */
typedef struct PartChannel0886A54C {
    unsigned char unk_00[0x50];
    short motion;                      /* +0x50 */
    unsigned char unk_52[0x60 - 0x52];
} PartChannel0886A54C;

extern void func_0886A948(D012ModelHolder *holder);
extern void func_0886A9B0(D012ModelHolder *holder, int channel, int motion, int frame);
extern int func_089B0D2C(void *model);                         /* current motion */
extern void *func_089B0F44(void *model, int motion);
extern float func_089B1870(void *motion);                      /* current frame */
extern void func_089B18FC(void *model, int motion, float frame);
extern void func_089B0D1C(void *motion, float frame);
extern void func_089B296C(void *model, float t);
extern void func_089B2C14(void *model);

/* model_part_switch: switch a multi-part holder to its pending part (+0x8D), carrying the
 * current motion and frame over to the new part's model (the parts are the GMOs of one model
 * file, e.g. a fighter's alternate bodies). */
void func_0886A54C(D012ModelHolder *holder)
{
    if (holder->part_count < 2) {
        return;
    }
    if (holder->pending_part < 0) {
        return;
    }
    {
        int count;
        int i;
        int offset;
        void *model;
        int motion;
        float frame;
        float zero;

        func_0886A948(holder);
        count = holder->current_part_addr->unk_04;
        for (i = 0, offset = 0; i < count; i++, offset += 0x60) {
            PartChannel0886A54C *channel = (PartChannel0886A54C *)((char *)holder->current_part_addr->unk_00 + offset);

            func_0886A9B0(holder, i, channel->motion, -1);
        }
        model = holder->parts[holder->pending_part].model_addr;
        motion = func_089B0D2C(holder->current_part_addr->model_addr);
        frame = func_089B1870(func_089B0F44(holder->current_part_addr->model_addr, motion));
        zero = 0.0f;
        func_089B18FC(model, motion, zero);
        func_089B0D1C(func_089B0F44(model, motion), frame);
        func_089B296C(model, zero);
        func_089B2C14(model);
        holder->current_part_addr = &holder->parts[holder->pending_part];
        holder->pending_part = -1;
    }
}

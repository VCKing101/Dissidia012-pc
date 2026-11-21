/* Dice effect start: clears the effect record, cues the guard banner when
 * the fighter is guarding, plays the hit motion, clears the eight face
 * slots and loads the dice sprites (the frame, five dice, the shadow)
 * before rolling the spin rate. */
#include "battle/agent31_types.h"

struct DiceStartParts {
    unsigned char pad_00[0x24];
    int faces[8];
    void *dice[5];
    void *shadow;
    void *frame;
};

struct DiceStart {
    A31VActor *actor;
    unsigned char pad_04[0x8];
    void *sheet;
    unsigned char pad_10[0x4];
    DiceStartParts *parts;
};

extern void *D_08A405E0;

extern "C" {

extern void func_08935828(void *dst, int value, int size);
extern void func_08A3DCA8(void *cue);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void *func_088DAA40(void *source, int name, int flags);
extern void *func_088DAA68(void *source);
extern void func_08A399D0(DiceStart *self);

void func_08A39BD8(DiceStart *self)
{
    int i;
    func_08935828(self->parts, 0, 0x60);
    if (self->actor->guard()) {
        func_08A3DCA8(D_08A405E0);
    }
    func_0882FA10(0.0f, 0.0f, self->actor->motion, 0x1FA5, 0);
    for (i = 0; i < 8; i++) {
        self->parts->faces[i] = 0;
    }
    self->parts->shadow = 0;
    self->parts->frame = func_088DAA40(self->sheet, 0, 0);
    for (i = 0; i < 5; i++) {
        self->parts->dice[i] = func_088DAA68(self->sheet);
    }
    self->parts->shadow = func_088DAA40(self->sheet, 2, 0);
    func_08A399D0(self);
}

}

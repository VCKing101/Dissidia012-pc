#include "battle/agent31_types.h"

/* Starts the dice effect: clears its parts, cues the guard banner when the
 * fighter guards, loads the dice and shadow sprites and the board, plays
 * the start motion and rolls the dice. */
struct DiceStartParts {
    int state;
    unsigned char pad_04[0x24];
    void *dice[3];
    void *shadows[3];
    void *board;
};

struct DiceStartEffect {
    A31VActor *actor;
    unsigned char pad_04[8];
    void *sheet;
    unsigned char pad_10[4];
    DiceStartParts *parts;
};

extern void *D_08A405E0;
extern "C" void func_0882FA10(float x, float y, void *motion, int id, int arg);

struct DiceMotion {
    static void play(A31VActor *actor, int id) { func_0882FA10(0.0f, 0.0f, actor->motion, id, 0); }
};

extern "C" {

extern void func_08935828(void *dest, int value, int size);
extern void func_08A3DCA8(void *banner);
extern void *func_088DAA68(void *sheet);
extern void *func_088DAA40(void *sheet, int frame, int flags);
extern void func_08A34F58(DiceStartEffect *self);

void func_08A35368(DiceStartEffect *self)
{
    func_08935828(self->parts, 0, 0x60);
    if (self->actor->guard()) {
        func_08A3DCA8(D_08A405E0);
    }
    for (int i = 0; i < 3; i++) {
        self->parts->dice[i] = func_088DAA68(self->sheet);
        self->parts->shadows[i] = func_088DAA68(self->sheet);
    }
    self->parts->board = func_088DAA40(self->sheet, 0x15, 0);
    DiceMotion::play(self->actor, 0x1FA5);
    func_08A34F58(self);
    self->parts->state = 0;
}

}

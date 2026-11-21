/* Pulse effect start: clears the effect record, cues the guard banner when
 * the fighter guards, plays the hit motion, loads the effect sprites (two
 * frames, five pairs of rings, the glow), sets up the pulse and plays its
 * sound unless the stage mutes it. */
#include "battle/agent31_types.h"

struct PulseStartParts {
    int state;
    unsigned char pad_04[0x1C];
    void *rings[5];
    void *glows[5];
    void *shadow;
    void *frame;
    void *frame_top;
};

struct PulseStage {
    unsigned char pad_00[0x14];
    unsigned char flags;

    static bool muted(PulseStage *stage) { return (stage->flags & 0x20) != 0; }
};

struct PulseStart {
    A31VActor *actor;
    unsigned char pad_04[0x8];
    PulseStage *sheet;
    unsigned char pad_10[0x4];
    PulseStartParts *parts;
};

extern void *D_08A405E0;

extern "C" {

extern void func_08935828(void *dst, int value, int size);
extern void func_08A3DCA8(void *cue);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void *func_088DAA40(void *source, int name, int flags);
extern void *func_088DAA68(void *source);
extern void *func_088A24D4(int owner, int id, int loop, int flags);
extern void func_08A3CA80(PulseStart *self);

void func_08A3CE14(PulseStart *self)
{
    int i;
    func_08935828(self->parts, 0, 0x64);
    if (self->actor->guard()) {
        func_08A3DCA8(D_08A405E0);
    }
    func_0882FA10(0.0f, 0.0f, self->actor->motion, 0x1FA5, 0);
    self->parts->frame = func_088DAA40(self->sheet, 0, 0);
    self->parts->frame_top = func_088DAA40(self->sheet, 1, 0);
    for (i = 0; i < 5; i++) {
        self->parts->rings[i] = func_088DAA68(self->sheet);
        self->parts->glows[i] = func_088DAA68(self->sheet);
    }
    self->parts->shadow = func_088DAA40(self->sheet, 2, 0);
    func_08A3CA80(self);
    if (!PulseStage::muted(self->sheet)) {
        func_088A24D4(0, 0x1E84D9, 1, 0);
    }
    self->parts->state = 0;
}

}

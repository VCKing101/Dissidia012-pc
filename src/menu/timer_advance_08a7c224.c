/* Runs a four-step reset once the timer has reached thirty frame-lengths, then advances the timer by the frame step. */
extern struct Step { char pad0[0x18]; float dt; } *D_089D69E0;
extern float D_089C9F54;
extern void func_08A6F170(int, int, int);
extern void func_08A72F5C(void *);
extern void func_08A743E0(void *);
extern void func_08A79E68(void *, int);
typedef struct Ot { char pad0[0x238]; int f238; } Ot;
void func_08A7C224(char *self, Ot *other) {
    float *tp = (float *)(self + 0x9B68);
    float thirty = 30.0f;
    float limit = D_089C9F54 * thirty;
    if (!(*tp < limit)) {
        func_08A6F170(other->f238, 5, 0);
        func_08A79E68((char *)other + 0x1B0, 0xF);
        func_08A743E0(self + 0x430);
        func_08A72F5C(self + 0x440);
    }
    *tp = *tp + D_089D69E0->dt;
}

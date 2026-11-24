/* Advances a float timer by the global delta; calls func_08B1E16C once it reaches its limit. Returns 0. */
extern void func_08B1E16C(void *self);
typedef struct { char pad[0x18]; float delta; } Clock;
extern Clock *D_089D69E0;
typedef struct { char pad[0x20]; float t; float limit; } Timer;
int func_08B1E584(Timer *self) {
    float n = self->t + D_089D69E0->delta;
    self->t = n;
    if (!(n < self->limit)) {
        func_08B1E16C(self);
    }
    return 0;
}

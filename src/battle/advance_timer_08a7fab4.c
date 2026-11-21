/* Resets the timer when the check says so (running a follow-up), then advances it by the frame step; returns 0. */
typedef struct Tm38 { char pad[0x9C]; float time; } Tm38;
typedef struct Frame38 { char pad[0x18]; float step; } Frame38;
extern int func_08A7EE04(Tm38 *self);
extern void func_08A7F4BC(Tm38 *self, int id);
extern Frame38 *D_089D69E0;
int func_08A7FAB4(Tm38 *self)
{
    if (func_08A7EE04(self) == 1) {
        self->time = 0.0f;
        func_08A7F4BC(self, 0x2711);
    }
    self->time = self->time + D_089D69E0->step;
    return 0;
}

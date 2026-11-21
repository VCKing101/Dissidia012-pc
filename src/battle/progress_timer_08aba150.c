/* While in state 1 advances the timer by the frame step and ends the state when it reaches the limit; -1 when inactive. */
typedef struct Tm38 { unsigned char active; char pad[7]; int state; float time; } Tm38;
typedef struct Frame38 { char pad[0x18]; float step; } Frame38;
extern float func_08ABA344(Tm38 *self);
extern Frame38 *D_089D69E0;
int func_08ABA150(Tm38 *self)
{
    float t;
    float total;
    if (self->active == 0) {
        return -1;
    }
    if (self->state == 1) {
        t = self->time + D_089D69E0->step;
        self->time = t;
        total = func_08ABA344(self);
        if (total <= t) {
            self->state = 0;
        }
    }
    return 0;
}

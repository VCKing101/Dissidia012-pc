// Counts down the timer at +0x238; when it has run out, restarts the state machine (0 if flag +0x299 was set, else 2); then subtracts the frame time and sets flag 0x20 in the owner's byte 0x5B3.
extern void func_08AE1270(void *, int);
extern float D_089D5EAC;
typedef struct Own57 {
    char pad0[0x10];
    unsigned char *target;
    char pad1[0x238 - 0x14];
    float timer;
    char pad2[0x299 - 0x23C];
    unsigned char flag;
} Own57;
void func_08AE0D70(Own57 *self)
{
    float *t = &self->timer;
    float cur = *t;
    float step = D_089D5EAC;
    if (cur <= 0.0f) {
        if (self->flag != 0) {
            self->flag = 0;
            func_08AE1270(self, 0);
            cur = *t;
        } else {
            func_08AE1270(self, 2);
            cur = *t;
        }
    }
    *t = cur - step;
    {
        unsigned char *fl = self->target + 0x5B0;
        fl[3] |= 0x20;
    }
}

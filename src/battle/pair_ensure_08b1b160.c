/* In state 1, ensures handles of kind 9 (at +0x20) and 0xF (at +0x24) exist, then moves to state 2 and refreshes; -1/-2/-3 on failure. */
typedef struct PHSub { char pad[1]; } PHSub;
typedef struct PHSlot { unsigned char active; char pad[3]; int state; char pad_08[0x18]; PHSub *h9; PHSub *hF; char pad_28[4]; int owner; } PHSlot;
extern PHSub *func_088DAA40(int owner, int kind, int z);
extern void func_088D9F5C(PHSub *h, int a, int b);
extern void func_08B1B398(PHSlot *s);
extern void func_08B1ACC4(PHSlot *s);
int func_08B1B160(PHSlot *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->state != 1) {
        return -2;
    }
    if (self->h9 == 0) {
        self->h9 = func_088DAA40(self->owner, 9, 0);
        if (self->h9 == 0) {
            return -3;
        }
    } else {
        func_088D9F5C(self->h9, 9, 1);
    }
    if (self->hF == 0) {
        self->hF = func_088DAA40(self->owner, 0xF, 0);
        if (self->hF == 0) {
            return -3;
        }
    }
    func_08B1B398(self);
    self->state = 2;
    func_08B1ACC4(self);
    return 0;
}

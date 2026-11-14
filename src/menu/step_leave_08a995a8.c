/* Leaves the menu step (state 0x23) unless flag 0x2000 is set, the
   object at +0x9C0 exists and func_08B2BC9C reports 0.
   (Not a stack-spill function: the early return written first gives retail's
   branch layout.) */
typedef struct Data08A995A8 {
    char unknown_0000[0x910];
    int state;
    char unknown_0914[0xAC];
    void *object;
    char unknown_09C4[0x77F0];
    unsigned flags;
} Data08A995A8;

typedef struct Menu08A995A8 {
    char unknown_000[0x154];
    Data08A995A8 *data;
} Menu08A995A8;

int func_08B2BC9C(void);
void func_08A9B354(Menu08A995A8 *menu);
void func_08A9D080(Menu08A995A8 *menu);

void func_08A995A8(Menu08A995A8 *self)
{
    if ((self->data->flags & 0x2000) != 0 && self->data->object != 0 && func_08B2BC9C() == 0) {
        return;
    }
    func_08A9B354(self);
    func_08A9D080(self);
    self->data->state = 0x23;
}

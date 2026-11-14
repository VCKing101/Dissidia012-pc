/* Advances the blink counter (every 30 steps it redraws) or redraws once when
   flag 0x10 is set, unless the menu is busy or flags block it. */
struct Blink08AB72AC {
    char pad_00[0x2C];
    unsigned int field_2C;
    unsigned int field_30;
    int field_34;
};

extern int func_08AB6DAC(struct Blink08AB72AC *menu);
extern void *func_08AB6D58(struct Blink08AB72AC *menu);
extern void func_08AB4990(struct Blink08AB72AC *menu);

void func_08AB72AC(struct Blink08AB72AC *self)
{
    if (func_08AB6DAC(self) == 1) {
        return;
    }
    func_08AB6D58(self);
    if ((self->field_2C & 0x20000) != 0) {
        return;
    }
    if ((self->field_30 & 0x20) != 0) {
        return;
    }
    if ((self->field_2C & 0x10) == 0) {
        self->field_34 = self->field_34 + 1;
        if (self->field_34 == 0x1E) {
            self->field_34 = 0;
            func_08AB4990(self);
        }
    } else if ((self->field_30 & 0x40) == 0) {
        func_08AB4990(self);
        self->field_30 = self->field_30 | 0x40;
    }
}

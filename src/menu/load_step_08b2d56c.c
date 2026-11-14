/* Polls the loader: 0 while it is not done, else closes it and maps its
   state 1/2/3 to 0 (after the follow-up step), 3 or 1; 2 otherwise. */
struct State08B2D56C {
    char pad_0[4];
    int field_4;
    int field_8;
};

struct Owner08B2D56C {
    char pad_000[0x844];
    struct State08B2D56C *field_844;
};

extern int func_08B2D710(struct Owner08B2D56C *self);
extern void func_08B2D744(struct Owner08B2D56C *self);
extern void func_08B2D7A4(struct Owner08B2D56C *self);

int func_08B2D56C(struct Owner08B2D56C *self)
{
    int result;
    if (func_08B2D710(self) != 1) {
        return 0;
    }
    func_08B2D744(self);
    result = 2;
    if (self->field_844->field_8 == 1) {
        func_08B2D7A4(self);
        self->field_844->field_4 = 8;
        result = 0;
    } else if (self->field_844->field_8 == 2) {
        result = 3;
    } else if (self->field_844->field_8 == 3) {
        result = 1;
    }
    return result;
}

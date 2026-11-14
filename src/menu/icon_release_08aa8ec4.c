/* Releases every icon handle, moves to state 6 and reports done. */
typedef struct Release2State {
    char pad_000[0x4];
    int field_04;
    char pad_008[0x490 - 0x8];
    void *field_490[0x2D];
} Release2State;

typedef struct Release2Owner {
    char pad_0000[0x7CFC];
    Release2State *field_7CFC;
} Release2Owner;

extern void func_08AA9604(Release2Owner *self);
extern void func_0887BA5C(void *handle);

int func_08AA8EC4(Release2Owner *self)
{
    int i;
    func_08AA9604(self);
    for (i = 0; i < 0x2D; i++) {
        if (self->field_7CFC->field_490[i] != 0) {
            func_0887BA5C(self->field_7CFC->field_490[i]);
            self->field_7CFC->field_490[i] = 0;
        }
    }
    self->field_7CFC->field_04 = 6;
    return 0;
}

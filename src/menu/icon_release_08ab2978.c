/* Saves the item block, releases the seven icon handles, moves to state 4 and reports done. */
typedef struct Release3State {
    char pad_000[0x4];
    int field_04;
    char pad_008[0xC - 0x8];
    void *field_0C;
    char field_10[0x480];
    void *field_490[7];
} Release3State;

typedef struct Release3Owner {
    char pad_000[0x584];
    Release3State *field_584;
} Release3Owner;

extern char D_08AB2F58[];
extern void func_088D3C24(void *file, void *block, void *name, int size);
extern void func_0887BA5C(void *handle);

int func_08AB2978(Release3Owner *self)
{
    int i;
    func_088D3C24(self->field_584->field_0C, self->field_584->field_10, D_08AB2F58, 0x120);
    for (i = 0; i < 7; i++) {
        if (self->field_584->field_490[i] != 0) {
            func_0887BA5C(self->field_584->field_490[i]);
            self->field_584->field_490[i] = 0;
        }
    }
    self->field_584->field_04 = 4;
    return 0;
}

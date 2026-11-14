/* Releases every icon handle of the page, then copies the saved layout index back. */
typedef struct Release4State {
    char pad_000[0x880];
    void *field_880[0xCE];
    char pad_BB8[0x255C - 0xBB8];
    int field_255C;
    int field_2560;
} Release4State;

typedef struct Release4Owner {
    char pad_0000[0xA690];
    Release4State *field_A690;
} Release4Owner;

extern void *D_08B01CF8;
extern void func_088D3A04(Release4State *state);
extern void func_08AF6DB8(void *renderer);
extern void func_0887BA5C(void *handle);

int func_08AE6B24(Release4Owner *self)
{
    int i;
    func_088D3A04(self->field_A690);
    func_08AF6DB8(D_08B01CF8);
    for (i = 0; i < 0xCE; i++) {
        if (self->field_A690->field_880[i] != 0) {
            func_0887BA5C(self->field_A690->field_880[i]);
            self->field_A690->field_880[i] = 0;
        }
    }
    self->field_A690->field_255C = self->field_A690->field_2560;
    return 0;
}

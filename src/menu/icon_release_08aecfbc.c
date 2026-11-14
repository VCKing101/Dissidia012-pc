/* Releases every icon handle of the page, then copies the saved layout index back. */
typedef struct Release4State {
    char pad_000[0x880];
    void *field_880[0x45];
    char pad_994[0x1928 - 0x994];
    int field_1928;
    int field_192C;
} Release4State;

typedef struct Release4Owner {
    char pad_0000[0x23F0];
    Release4State *field_23F0;
} Release4Owner;

extern void *D_08B01CF8;
extern void func_088D3A04(Release4State *state);
extern void func_08AF6DB8(void *renderer);
extern void func_0887BA5C(void *handle);

int func_08AECFBC(Release4Owner *self)
{
    int i;
    func_088D3A04(self->field_23F0);
    func_08AF6DB8(D_08B01CF8);
    for (i = 0; i < 0x45; i++) {
        if (self->field_23F0->field_880[i] != 0) {
            func_0887BA5C(self->field_23F0->field_880[i]);
            self->field_23F0->field_880[i] = 0;
        }
    }
    self->field_23F0->field_1928 = self->field_23F0->field_192C;
    return 0;
}

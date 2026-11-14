/* Releases every icon handle of the page, then copies the saved layout index back. */
typedef struct Release4State {
    char pad_000[0x880];
    void *field_880[0xA9];
    char pad_B24[0x1B80 - 0xB24];
    int field_1B80;
    int field_1B84;
} Release4State;

typedef struct Release4Owner {
    char pad_0000[0x46AC];
    Release4State *field_46AC;
} Release4Owner;

extern void *D_08B01CF8;
extern void func_088D3A04(Release4State *state);
extern void func_08AF6DB8(void *renderer);
extern void func_0887BA5C(void *handle);

int func_08ADC760(Release4Owner *self)
{
    int i;
    func_088D3A04(self->field_46AC);
    func_08AF6DB8(D_08B01CF8);
    for (i = 0; i < 0xA9; i++) {
        if (self->field_46AC->field_880[i] != 0) {
            func_0887BA5C(self->field_46AC->field_880[i]);
            self->field_46AC->field_880[i] = 0;
        }
    }
    self->field_46AC->field_1B80 = self->field_46AC->field_1B84;
    return 0;
}

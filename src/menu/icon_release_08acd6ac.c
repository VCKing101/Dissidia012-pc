/* Releases every icon handle of the page, then copies the saved layout index back. */
typedef struct Release4State {
    char pad_000[0x880];
    void *field_880[0xC3];
    char pad_B8C[0xF1C - 0xB8C];
    int field_F1C;
    int field_F20;
} Release4State;

typedef struct Release4Owner {
    char pad_0000[0xDB30];
    Release4State *field_DB30;
} Release4Owner;

extern void *D_08B01CF8;
extern void func_088D3A04(Release4State *state);
extern void func_08AF6DB8(void *renderer);
extern void func_0887BA5C(void *handle);

int func_08ACD6AC(Release4Owner *self)
{
    int i;
    func_088D3A04(self->field_DB30);
    func_08AF6DB8(D_08B01CF8);
    for (i = 0; i < 0xC3; i++) {
        if (self->field_DB30->field_880[i] != 0) {
            func_0887BA5C(self->field_DB30->field_880[i]);
            self->field_DB30->field_880[i] = 0;
        }
    }
    self->field_DB30->field_F1C = self->field_DB30->field_F20;
    return 0;
}

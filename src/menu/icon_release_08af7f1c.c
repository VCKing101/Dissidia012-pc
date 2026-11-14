/* Releases every icon handle of the page, then its effect. */
typedef struct Release5State {
    char pad_000[0x8A8];
    void *field_8A8[0x5D];
    char pad_A1C[0xC08 - 0xA1C];
    void *field_C08;
} Release5State;

typedef struct Release5Owner {
    char pad_000[0x140];
    Release5State *field_140;
} Release5Owner;

extern void func_0887BA5C(void *handle);
extern void func_08AF091C(void *effect);

void func_08AF7F1C(Release5Owner *self)
{
    int i;
    for (i = 0; i < 0x5D; i++) {
        if (self->field_140->field_8A8[i] != 0) {
            func_0887BA5C(self->field_140->field_8A8[i]);
            self->field_140->field_8A8[i] = 0;
        }
    }
    if (self->field_140->field_C08 != 0) {
        func_08AF091C(self->field_140->field_C08);
    }
}

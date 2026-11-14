/* Releases every icon handle, then copies the saved layout index back. */
typedef struct ReleaseState {
    char pad_000[0x880];
    void *field_880[(0xC88 - 0x880) / 4];
    int field_C88;
    int field_C8C;
} ReleaseState;

typedef struct ReleaseOwner {
    char pad_0000[0xD80];
    ReleaseState *field_D80;
} ReleaseOwner;

extern void func_088D3A04(ReleaseState *state);
extern void func_0887BA5C(void *handle);

void func_08A166B0(ReleaseOwner *self)
{
    int i;
    func_088D3A04(self->field_D80);
    for (i = 0; i < 0x6A; i++) {
        if (self->field_D80->field_880[i] != 0) {
            func_0887BA5C(self->field_D80->field_880[i]);
            self->field_D80->field_880[i] = 0;
        }
    }
    self->field_D80->field_C88 = self->field_D80->field_C8C;
}

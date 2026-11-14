/* Releases every icon handle, then copies the saved layout index back. */
typedef struct ReleaseState {
    char pad_000[0x880];
    void *field_880[(0x10F0 - 0x880) / 4];
    int field_10F0;
    int field_10F4;
} ReleaseState;

typedef struct ReleaseOwner {
    char pad_0000[0x17B0];
    ReleaseState *field_17B0;
} ReleaseOwner;

extern void func_088D3A04(ReleaseState *state);
extern void func_0887BA5C(void *handle);

void func_08A19308(ReleaseOwner *self)
{
    int i;
    func_088D3A04(self->field_17B0);
    for (i = 0; i < 0x51; i++) {
        if (self->field_17B0->field_880[i] != 0) {
            func_0887BA5C(self->field_17B0->field_880[i]);
            self->field_17B0->field_880[i] = 0;
        }
    }
    self->field_17B0->field_10F0 = self->field_17B0->field_10F4;
}

/* Releases every icon handle, then copies the saved layout index back. */
typedef struct ReleaseState {
    char pad_000[0x880];
    void *field_880[(0x1E74 - 0x880) / 4];
    int field_1E74;
    int field_1E78;
} ReleaseState;

typedef struct ReleaseOwner {
    char pad_0000[0x28F0];
    ReleaseState *field_28F0;
} ReleaseOwner;

extern void func_088D3A04(ReleaseState *state);
extern void func_0887BA5C(void *handle);

void func_08A2498C(ReleaseOwner *self)
{
    int i;
    func_088D3A04(self->field_28F0);
    for (i = 0; i < 0x92; i++) {
        if (self->field_28F0->field_880[i] != 0) {
            func_0887BA5C(self->field_28F0->field_880[i]);
            self->field_28F0->field_880[i] = 0;
        }
    }
    self->field_28F0->field_1E74 = self->field_28F0->field_1E78;
}

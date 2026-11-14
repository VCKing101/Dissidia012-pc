/* Releases every icon handle, then copies the saved layout index back. */
typedef struct ReleaseState {
    char pad_000[0x880];
    void *field_880[471];
    int field_FDC;
    int field_FE0;
} ReleaseState;

typedef struct ReleaseOwner {
    char pad_0000[0x1320];
    ReleaseState *field_1320;
} ReleaseOwner;

extern void func_088D3A04(ReleaseState *state);
extern void func_0887BA5C(void *handle);

void func_08A0DD88(ReleaseOwner *self)
{
    int i;
    func_088D3A04(self->field_1320);
    for (i = 0; i < 0x62; i++) {
        if (self->field_1320->field_880[i] != 0) {
            func_0887BA5C(self->field_1320->field_880[i]);
            self->field_1320->field_880[i] = 0;
        }
    }
    self->field_1320->field_FDC = self->field_1320->field_FE0;
}

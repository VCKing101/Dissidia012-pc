/* Releases every icon handle, then copies the saved layout index back. */
typedef struct ReleaseState {
    char pad_000[0x880];
    void *field_880[261];
    int field_C94;
    int field_C98;
} ReleaseState;

typedef struct ReleaseOwner {
    char pad_0000[0xF60];
    ReleaseState *field_F60;
} ReleaseOwner;

extern void func_088D3A04(ReleaseState *state);
extern void func_0887BA5C(void *handle);
extern void *D_08A23FF0;
extern void func_08A1E0EC(void *renderer);

void func_08A129B4(ReleaseOwner *self)
{
    int i;
    func_088D3A04(self->field_F60);
    func_08A1E0EC(D_08A23FF0);
    for (i = 0; i < 0x42; i++) {
        if (self->field_F60->field_880[i] != 0) {
            func_0887BA5C(self->field_F60->field_880[i]);
            self->field_F60->field_880[i] = 0;
        }
    }
    self->field_F60->field_C94 = self->field_F60->field_C98;
}

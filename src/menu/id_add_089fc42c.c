typedef struct IdState {
    char pad_0000[0x2E84];
    int field_2E84[5];
    int field_2E98[5];
    int field_2EAC[0x23];
} IdState;

typedef struct IdOwner {
    char pad_000[0x14C];
    IdState *field_14C;
} IdOwner;

int func_089FC42C(IdOwner *self, int id)
{
    int result = 0;
    int i;
    for (i = 0; i < 0x23; i++) {
        if (self->field_14C->field_2EAC[i] == id) {
            break;
        }
        if (self->field_14C->field_2EAC[i] == 0) {
            self->field_14C->field_2EAC[i] = id;
            result = 1;
            break;
        }
    }
    return result;
}

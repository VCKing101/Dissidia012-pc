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

int func_089FC218(IdOwner *self, int id)
{
    int result = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->field_14C->field_2E84[i] == 0) {
            break;
        }
        if (self->field_14C->field_2E84[i] == id) {
            result = 1;
            break;
        }
    }
    return result;
}

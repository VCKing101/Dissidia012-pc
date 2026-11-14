/* Whether the page is in state 6. */
typedef struct StateIs08AF9FB4 {
    int field_00;
    int field_04;
} StateIs08AF9FB4;

typedef struct StateIsOwner08AF9FB4 {
    char pad_000[0x140];
    StateIs08AF9FB4 *field_140;
} StateIsOwner08AF9FB4;

int func_08AF9FB4(StateIsOwner08AF9FB4 *self)
{
    int result = 0;
    if (self->field_140->field_04 == 6) {
        result = 1;
    }
    return result;
}

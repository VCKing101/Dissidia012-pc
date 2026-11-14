/* Whether the page is in state 5. */
typedef struct StateIs08AF9F94 {
    int field_00;
    int field_04;
} StateIs08AF9F94;

typedef struct StateIsOwner08AF9F94 {
    char pad_000[0x140];
    StateIs08AF9F94 *field_140;
} StateIsOwner08AF9F94;

int func_08AF9F94(StateIsOwner08AF9F94 *self)
{
    int result = 0;
    if (self->field_140->field_04 == 5) {
        result = 1;
    }
    return result;
}

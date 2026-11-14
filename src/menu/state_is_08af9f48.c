/* Whether the page is in state 4. */
typedef struct StateIs08AF9F48 {
    int field_00;
    int field_04;
} StateIs08AF9F48;

typedef struct StateIsOwner08AF9F48 {
    char pad_000[0x140];
    StateIs08AF9F48 *field_140;
} StateIsOwner08AF9F48;

int func_08AF9F48(StateIsOwner08AF9F48 *self)
{
    int result = 0;
    if (self->field_140->field_04 == 4) {
        result = 1;
    }
    return result;
}

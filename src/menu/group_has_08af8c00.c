/* Whether one of the five groups contains the id. */
typedef struct HasGroup08AF8C00 {
    char data[0xC6];
} HasGroup08AF8C00;

typedef struct HasGroupState08AF8C00 {
    char pad_000[0xC80];
    HasGroup08AF8C00 field_C80[5];
} HasGroupState08AF8C00;

typedef struct HasGroupOwner08AF8C00 {
    char pad_000[0x140];
    HasGroupState08AF8C00 *field_140;
} HasGroupOwner08AF8C00;

extern int func_088B7A48(HasGroup08AF8C00 *group, short id);

int func_08AF8C00(HasGroupOwner08AF8C00 *self, short id)
{
    int result = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (func_088B7A48(&self->field_140->field_C80[i], id) != 0) {
            result = 1;
            break;
        }
    }
    return result;
}

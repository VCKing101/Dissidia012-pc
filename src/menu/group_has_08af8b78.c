/* Whether one of the five groups contains the id. */
typedef struct HasGroup08AF8B78 {
    char data[0xC6];
} HasGroup08AF8B78;

typedef struct HasGroupState08AF8B78 {
    char pad_000[0xC80];
    HasGroup08AF8B78 field_C80[5];
} HasGroupState08AF8B78;

typedef struct HasGroupOwner08AF8B78 {
    char pad_000[0x140];
    HasGroupState08AF8B78 *field_140;
} HasGroupOwner08AF8B78;

extern int func_088B7A08(HasGroup08AF8B78 *group, unsigned short id);

int func_08AF8B78(HasGroupOwner08AF8B78 *self, unsigned short id)
{
    int result = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (func_088B7A08(&self->field_140->field_C80[i], id) != 0) {
            result = 1;
            break;
        }
    }
    return result;
}

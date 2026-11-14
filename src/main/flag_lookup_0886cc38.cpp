extern "C" {
/* The flag of the entry with the id; 1 when there is none. */
typedef struct Entry0886CC38 {
    unsigned char id;
    unsigned char flag;
    unsigned char field_02;
} Entry0886CC38;

typedef struct Owner0886CC38 {
    char pad_000[0x8E];
    unsigned char count;
    char pad_08F[0x7D];
    Entry0886CC38 *entries;
} Owner0886CC38;

int func_0886CC38(Owner0886CC38 *o, unsigned int id)
{
    for (int i = 0; i < o->count; i++) {
        if (o->entries[i].id == id) {
            return o->entries[i].flag != 0;
        }
    }
    return 1;
}
}

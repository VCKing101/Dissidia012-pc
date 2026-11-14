/* Stores a source's id, level and value in the first free entry of four. */
typedef struct Entry0880AAE0 {
    int id;
    float level;
    int value;
} Entry0880AAE0;

typedef struct Owner0880AAE0 {
    char pad_00[0x48];
    Entry0880AAE0 entries[4];
} Owner0880AAE0;

typedef struct Source0880AAE0 {
    char pad_00[0x18];
    int value;
    char pad_1C[0xA];
    short id;
} Source0880AAE0;

typedef struct Info0880AAE0 {
    char pad_00[0x12];
    unsigned char level;
} Info0880AAE0;

void func_0880AAE0(Owner0880AAE0 *self, Source0880AAE0 *src, Info0880AAE0 *info)
{
    short id = src->id;
    int i;
    for (i = 0; i < 4; i++) {
        if (self->entries[i].id == 0) {
            self->entries[i].id = id;
            self->entries[i].level = info->level;
            self->entries[i].value = src->value;
            break;
        }
    }
}

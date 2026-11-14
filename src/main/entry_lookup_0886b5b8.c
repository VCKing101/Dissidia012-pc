typedef struct Info0886B5B8 {
    unsigned char unknown_00[8];
    int table;
} Info0886B5B8;

typedef struct Owner0886B5B8 {
    unsigned char unknown_00[0x84];
    Info0886B5B8 *info;
    unsigned char unknown_88[0x5C];
    int index;
} Owner0886B5B8;

extern unsigned char D_09AF8AC0[];
int func_089B0E78(int table, int index);
void *func_089B1224(int entry);

void *func_0886B5B8(Owner0886B5B8 *self) {
    if (self->index >= 0) {
        int table = self->info->table;
        return func_089B1224(func_089B0E78(table, self->index));
    } else {
        return D_09AF8AC0;
    }
}

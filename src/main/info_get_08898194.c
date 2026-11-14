typedef struct Info08898194 {
    unsigned char unknown_00[0x08];
    int value;
} Info08898194;

typedef struct Owner08898194 {
    unsigned char unknown_00[0x58];
    Info08898194 *info;
} Owner08898194;

int func_08898194(Owner08898194 *self) {
    Info08898194 *info = self->info;
    if (info != 0) {
        return info->value;
    }
    return 0;
}

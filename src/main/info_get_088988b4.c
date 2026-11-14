typedef struct Info088988B4 {
    unsigned char unknown_00[0x14];
    int value;
} Info088988B4;

typedef struct Owner088988B4 {
    unsigned char unknown_00[0x58];
    Info088988B4 *info;
} Owner088988B4;

int func_088988B4(Owner088988B4 *self) {
    Info088988B4 *info = self->info;
    if (info != 0) {
        return info->value;
    }
    return 0;
}

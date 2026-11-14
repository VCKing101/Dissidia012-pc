typedef struct Info08897EFC {
    int value;
} Info08897EFC;

typedef struct Owner08897EFC {
    unsigned char unknown_00[0x58];
    Info08897EFC *info;
} Owner08897EFC;

int func_08897EFC(Owner08897EFC *self) {
    Info08897EFC *info = self->info;
    if (info != 0) {
        return info->value;
    }
    return 0;
}

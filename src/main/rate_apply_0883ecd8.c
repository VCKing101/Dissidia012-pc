typedef struct Modifier0883ECD8 {
    int field_00;
    short percent;
    unsigned char unknown_06[6];
    struct Modifier0883ECD8 *next;
} Modifier0883ECD8;

void func_0883ECD8(int *value, Modifier0883ECD8 *mod) {
    short *percent;
    for (; mod != 0; mod = mod->next) {
        percent = &mod->percent;
        *value = *value * *percent / 100;
    }
}

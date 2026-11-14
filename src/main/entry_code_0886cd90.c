typedef struct Owner0886CD90 {
    unsigned char unknown_00[0xC];
    int table;
} Owner0886CD90;

void *func_089B0E78(int table);
int func_089B0F98(void *entry);

int func_0886CD90(Owner0886CD90 *self) {
    void *entry = func_089B0E78(self->table);
    int code;
    if (entry == 0) {
        return -1;
    }
    code = func_089B0F98(entry) & 0xFFF;
    if (code == 0xFFF) {
        return -1;
    }
    return code;
}

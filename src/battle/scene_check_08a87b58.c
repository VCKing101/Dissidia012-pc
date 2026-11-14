typedef struct SceneCheck {
    unsigned char field_00;
    unsigned char enabled;
    char pad_02[0x2A];
    char field_2C[0x14];
    char field_40[0xF0];
    char field_130[0x4];
} SceneCheck;

extern unsigned char *D_08AAAD80;
extern int func_08A7E7FC(void *sub);
extern int func_08A95890(void *sub);

int func_08A87B58(SceneCheck *self)
{
    if (!*D_08AAAD80) {
        self->enabled = 0;
        return 0;
    }
    self->enabled = 1;
    if (func_08A7E7FC(self->field_2C) != 0) {
        return -1;
    }
    if (func_08A95890(self->field_40) != 0) {
        return -2;
    }
    if (func_08A95890(self->field_130) != 0) {
        return -3;
    }
    return 0;
}

typedef struct MenuSaveFlags {
    char pad_00[0x13];
    unsigned char field_13;
} MenuSaveFlags;

extern MenuSaveFlags D_09B6DCE8;
extern void func_08A4EA6C(void *self);

void func_08A4EA48(void *self) {
    D_09B6DCE8.field_13 = 0;
    func_08A4EA6C(self);
}

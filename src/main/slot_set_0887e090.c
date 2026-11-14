typedef struct Slot0887E090 {
    int value;
    unsigned char unknown_04;
    unsigned char kind;
    unsigned short extra;
} Slot0887E090;

typedef struct Owner0887E090 {
    unsigned char unknown_00[0x30];
    Slot0887E090 slots[2];
} Owner0887E090;

void func_0887E090(Owner0887E090 *self, int value, int index, unsigned char kind, unsigned short extra) {
    if (index >= 2) {
        return;
    }
    self->slots[index].value = value;
    self->slots[index].kind = kind;
    self->slots[index].extra = extra;
}

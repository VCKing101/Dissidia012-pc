typedef struct Slot0887E0DC {
    int value;
    unsigned char unknown_04;
    unsigned char kind;
    unsigned short extra;
} Slot0887E0DC;

typedef struct Owner0887E0DC {
    unsigned char unknown_00[0x30];
    Slot0887E0DC slots[2];
} Owner0887E0DC;

void func_0887E0DC(Owner0887E0DC *self, int index) {
    if (index >= 2) {
        return;
    }
    self->slots[index].value = 0;
    self->slots[index].kind = 0x12;
    self->slots[index].extra = 0x80;
}

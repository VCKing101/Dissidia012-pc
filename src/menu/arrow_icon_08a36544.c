/* Sets the arrow icon for the panel mode and direction (frame 0x2C left, 0x30 right). */
typedef struct ArrowPanel {
    char pad_00[0x4];
    void **field_04;
    char pad_08[0x6C - 0x8];
    int field_6C;
} ArrowPanel;

typedef struct ArrowOwner {
    char pad_000[0xF64];
    ArrowPanel *field_F64;
} ArrowOwner;

extern void func_088D9F5C(void *icon, int frame, int one);

void func_08A36544(ArrowOwner *self, int direction)
{
    int index = 0;
    int frame = 0x30;
    if (direction < 0) {
        frame = 0x2C;
    }
    switch (self->field_F64->field_6C) {
    case 0:
        index = direction < 0 ? 2 : 3;
        break;
    case 1:
        index = direction < 0 ? 4 : 5;
        break;
    }
    func_088D9F5C(self->field_F64->field_04[index], frame, 1);
}

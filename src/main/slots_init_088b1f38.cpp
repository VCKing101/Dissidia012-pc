/* Initialises the ten 0x44-byte slots that start at offset 4. */
struct Slot088B1F38 {
    char data[0x44];
};

struct Slots088B1F38 {
    int field_00;
    Slot088B1F38 slots[10];
};

extern "C" {

void func_088BD564(Slot088B1F38 *slot);

void func_088B1F38(Slots088B1F38 *self)
{
    for (int i = 0; i < 10; i++) {
        func_088BD564(&self->slots[i]);
    }
}

}

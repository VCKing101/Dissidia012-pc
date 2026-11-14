typedef struct SlotList8 {
    int pad_00;
    int slots[8];
} SlotList8;

/* Put value into the first empty slot. */
void func_08AB7EE8(SlotList8 *self, int value)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (self->slots[i] == 0) {
            self->slots[i] = value;
            break;
        }
    }
}

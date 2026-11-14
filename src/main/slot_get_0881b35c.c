typedef struct Agent4Slot40 {
    unsigned char data[0x40];
} Agent4Slot40;

typedef struct Agent4SlotPool {
    unsigned char valid;
    unsigned char unknown_01[3];
    unsigned int capacity;
    Agent4Slot40 *slots;
} Agent4SlotPool;

Agent4Slot40 *func_0881B35C(Agent4SlotPool *self, unsigned int index) {
    if (!self->valid) {
        return 0;
    }
    if (index >= self->capacity) {
        return 0;
    }
    return &self->slots[index];
}

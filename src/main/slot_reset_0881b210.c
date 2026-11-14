typedef struct Agent4Slot40 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    unsigned char unknown_10[0x30];
} Agent4Slot40;

typedef struct Agent4SlotPool {
    unsigned char valid;
    unsigned char unknown_01[3];
    unsigned int capacity;
    Agent4Slot40 *slots;
    int *used;
    int used_count;
} Agent4SlotPool;

int func_0881B210(Agent4SlotPool *self) {
    if (!self->valid) {
        return -1;
    }
    {
        int i;
        for (i = 0; i < self->used_count; i++) {
            self->slots[self->used[i]].field_0C = 0;
        }
    }
    self->used_count = 0;
    return 0;
}

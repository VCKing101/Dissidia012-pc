typedef struct Agent4HitSlot {
    unsigned char unknown_00[0x20];
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    short field_30;
    short field_32;
    unsigned char field_34;
    unsigned char field_35;
    short field_36;
    unsigned char unknown_38[8];
} Agent4HitSlot;

typedef struct Agent4HitQueue {
    unsigned char unknown_0000[0x180];
    Agent4HitSlot slots[6];
    unsigned char unknown_0300;
    unsigned char count;
    unsigned char unknown_0302;
    unsigned char reset;
} Agent4HitQueue;

void func_088395B8(Agent4HitSlot *slot, int a, int b, int c);

void func_088394B8(Agent4HitQueue *self, int a, int b, int c) {
    if (self->reset) {
        int i;
        Agent4HitSlot *slot;
        self->count = 0;
        for (i = 0; i < 6; i++) {
            slot = &self->slots[i];
            slot->field_20 = 0;
            slot->field_24 = 0;
            slot->field_28 = 0;
            slot->field_2C = 0;
            slot->field_30 = 0;
            slot->field_32 = 100;
            slot->field_34 = 0;
            slot->field_35 = 0;
            slot->field_36 = -1;
        }
        self->reset = 0;
    }
    if (self->count < 6) {
        func_088395B8(&self->slots[self->count], a, b, c);
        self->count++;
    }
}

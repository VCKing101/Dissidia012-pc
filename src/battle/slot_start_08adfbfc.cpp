typedef struct StartSlot {
    char pad_00[0x8];
    void *owner;
    char pad_0C[0xC];

    static unsigned char isUsed(StartSlot *slot)
    {
        unsigned int used = slot->owner != 0;
        return used;
    }
} StartSlot;

typedef struct StartSlots {
    StartSlot slots[3];
} StartSlots;

extern "C" {

extern void func_08ADF460(StartSlot *slot, int a, int b, int c, unsigned char d, unsigned char e);

void func_08ADFBFC(StartSlots *self, int a, int b, int c, unsigned char d, unsigned char e)
{
    for (int i = 0; i < 3; i++) {
        if (!StartSlot::isUsed(&self->slots[i])) {
            func_08ADF460(&self->slots[i], a, b, c, d, e);
            return;
        }
    }
}

}

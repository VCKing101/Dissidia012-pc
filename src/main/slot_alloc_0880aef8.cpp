/* Hands out the next free task slot (1-255) from the global slot set,
   searching round-robin from the last one handed out; 0 when all are used. */
extern "C" {

extern unsigned int D_089D4750[8];
extern int D_08B63284;

}

struct TaskSlots0880AEF8 {
    static bool used(unsigned int slot) { return (*(unsigned int *)((char *)D_089D4750 + (slot >> 5) * 4) & (1 << (slot & 0x1F))) != 0; }
    static void take(unsigned int slot) { D_089D4750[slot >> 5] |= 1 << (slot & 0x1F); }
};

extern "C" {

int func_0880AEF8(void)
{
    int i = 0;
    do {
        D_08B63284 = (D_08B63284 + 1) % 255;
        if (!TaskSlots0880AEF8::used(D_08B63284 + 1)) {
            TaskSlots0880AEF8::take(D_08B63284 + 1);
            return D_08B63284 + 1;
        }
        i++;
    } while (i < 256);
    return 0;
}

}

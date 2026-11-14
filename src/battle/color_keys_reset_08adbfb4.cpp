typedef struct ColorKey {
    short base;
    short value;
    unsigned short flags;
} ColorKey;

extern "C" {

extern void func_088B766C(void *target, int index, int value);

/* Reset every flagged key on target. */
void func_08ADBFB4(void *self, void *target, ColorKey *keys)
{
    for (int i = 0; i < 10; i++) {
        if (keys[i].flags & 1) {
            func_088B766C(target, i, -1);
        }
    }
}

}

typedef struct ColorKey {
    short base;
    short value;
    unsigned short flags;
} ColorKey;

extern "C" {

extern void func_088B766C(void *target, int index, int value);

/* Apply the ten base values to target. */
void func_08ADBF5C(void *self, void *target, ColorKey *keys)
{
    for (int i = 0; i < 10; i++) {
        func_088B766C(target, i, keys[i].base);
    }
}

}

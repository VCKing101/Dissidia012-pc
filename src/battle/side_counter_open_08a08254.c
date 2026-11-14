/* Opens a side counter with the digit set for the side and kind. */
typedef struct SideCounter {
    unsigned char open;
    unsigned char pad_01[3];
    unsigned char digits[4];
} SideCounter;

extern void func_08A08224(SideCounter *counter);
extern unsigned int func_08A22CA8(unsigned int side);
extern int func_08A10BE0(void *digits, void *source, int a, void *layout, int b, int set, int c);
extern int D_08A08398[2][4];

int func_08A08254(SideCounter *counter, unsigned int side, unsigned int kind, void *source)
{
    unsigned int colour;

    func_08A08224(counter);
    if (side >= 2 || kind >= 4 || source == 0) {
        return -1;
    }
    colour = func_08A22CA8(side);
    if (colour >= 2) {
        return -2;
    }
    if (func_08A10BE0(counter->digits, source, 1, source, 0, D_08A08398[colour][kind], 1) != 0) {
        return -3;
    }
    counter->open = 1;
    return 0;
}

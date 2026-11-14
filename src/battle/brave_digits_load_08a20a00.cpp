#include "battle/agent28_types.h"

/* Loads the Brave counter's frame and one digit model for each digit of the
 * side's Brave; leading zeros get no model. */
struct BraveDigits {
    unsigned char open;
    unsigned char pad_01[3];
    void *frame;
    void *digits[4];
    unsigned int side;
    void *source;
};

extern "C" {

extern RootHolder *D_089D6A10;
extern int D_08A20B68[2];
extern unsigned int func_08A22CA8(unsigned int side);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_088DA060(void *model);
extern int func_089DDBA8(void *root, unsigned int side);
extern void func_08A20850(BraveDigits *digits);

int func_08A20A00(BraveDigits *counter)
{
    if (!counter->open) {
        return -1;
    }
    if (counter->frame == 0) {
        void *root = D_089D6A10->root;
        if (root == 0) {
            return -2;
        }
        unsigned int kind = func_08A22CA8(counter->side);
        if (kind >= 2) {
            return -3;
        }
        counter->frame = func_088DAA40(counter->source, D_08A20B68[kind], 0);
        if (counter->frame == 0) {
            return -4;
        }
        int brave = func_089DDBA8(root, counter->side);
        for (int i = 0; i < 4; i++) {
            if (counter->digits[i] != 0) {
                func_088DA060(counter->digits[i]);
                counter->digits[i] = 0;
            }
            if (brave == 0 && i != 0) {
                continue;
            }
            counter->digits[i] = func_088DAA40(counter->source, 4, 0);
            brave /= 10;
        }
        func_08A20850(counter);
    }
    return 0;
}

}

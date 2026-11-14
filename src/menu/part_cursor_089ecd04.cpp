/* Arcade menu part: moves the slot cursor by `step` (wrapping), flashes the
   arrow on that side and moves the highlight from the old slot to the new
   one. Returns 1 when the cursor moved. */
struct PartSlot29 {
    char pad_00[0x10];
    void *model;
    char pad_14[0x28];
};

struct ArcadePart29 {
    char pad_00[0x50];
    void *left;          /* +0x50 */
    void *right;         /* +0x54 */
    char pad_58[0x8];
    PartSlot29 slots[3]; /* +0x60 */
    char pad_114[0x3C];
    int count;           /* +0x150 */
    int cursor;          /* +0x154 */
};

extern "C" {
void func_088D9F5C(void *model, int motion, bool arg);

int func_089ECD04(ArcadePart29 *self, int step)
{
    int old = self->cursor;
    int result = 0;

    self->cursor = (old + self->count + step) % self->count;
    if (self->cursor != old) {
        if (step > 0) {
            if (self->right != 0) {
                func_088D9F5C(self->right, 0xC, true);
                *(short *)((char *)self->right + 0xE) = 0xB;
            }
        } else {
            if (self->left != 0) {
                func_088D9F5C(self->left, 9, true);
                *(short *)((char *)self->left + 0xE) = 8;
            }
        }
        void *model = self->slots[old].model;
        if (model != 0) {
            func_088D9F5C(model, 0xB, true);
        }
        void *next = self->slots[self->cursor].model;
        if (next != 0) {
            func_088D9F5C(next, 9, true);
            *(short *)((char *)next + 0xE) = 0xA;
        }
        result = 1;
    }
    return result;
}
}

/* Finds the index of a value in the page's id list (count depends on page kind) and sets the cursor range to it. */
typedef struct Fc { char pad0[0x14]; int kind; char pad1[0x74 - 0x18]; unsigned flags; char pad2[0x4A4 - 0x78]; int ids[1]; } Fc;
extern int func_08A64308(Fc *);
extern int func_08A643E8(Fc *);
extern void func_088D46E4(void *, int, int, int, int, int);
void func_08A64240(Fc *self, int value) {
    int count = 0;
    int i;
    int found;
    if (self->kind <= 0) {
        if (self->kind >= 0) {
            count = func_08A64308(self);
        }
    } else if (self->kind < 2) {
        count = func_08A643E8(self);
    }
    found = 0;
    i = 0;
    if (i < count) {
        do {
            if (self->ids[i] == value) {
                found = i;
                break;
            }
            i++;
        } while (i < count);
    }
    func_088D46E4(&self->flags, found, 0, count - 1, 0, -1);
    self->flags |= 1;
}

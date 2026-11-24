// Stores a selection id, finds the slot whose id matches (or the first), and resets the cursor state.
struct Self {
    char pad0[0x28]; int mode;
    char pad1[0x40 - 0x2C]; int h40;
    char pad2[0xBC - 0x44]; char slots[0x4C];
    char pad3[0x4E4 - 0xBC - 0x4C]; int count; int sel; char cur[0x4]; int curIdx;
    char pad4[0x518 - 0x4F4]; int f518; char pad5[4]; int f520; char blob[1];
};
extern void func_088D9CD8(int);
extern int func_08B475DC(void *);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_088D4788(void *, void *, int);
extern void func_08B4753C(void *);

void func_08B47D24(struct Self *self, int id, int want)
{
    int found;
    int count;
    self->sel = id;
    func_088D9CD8(self->h40);
    found = 0;
    count = self->count;
    if (want != -1) {
        int i = 0;
        char *s = self->slots;
        for (; i < self->count; i++, s += 0x4C) {
            if (func_08B475DC(s) == want) {
                found = i;
            }
        }
        count = self->count;
    }
    func_088D46E4(self->cur, found, 0, count - 1, 0, -1);
    func_088D4788(self->cur, self->blob, self->f520);
    func_08B4753C((char *)self + self->curIdx * 0x4C + 0xBC);
    self->f518 = 0;
    self->mode = 2;
}

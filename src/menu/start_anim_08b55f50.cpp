/* Applies one of two ids (by the index at +0x3C) with func_088D9F5C on the object at +8, calls func_088D9CFC on each non-zero handle of five words from +0x1C, then clears the word at +0x50. */
struct S { char pad[8]; int obj; char pad2[0x3C - 0xC]; int idx; };
extern "C" void func_088D9F5C(int obj, int id, int one);
extern "C" void func_088D9CFC(int h);
extern "C" void func_08B55F50(S *self)
{
    int t[2] = { 0x11, 0x16 };
    int idx = self->idx;
    int id = t[idx];
    func_088D9F5C(self->obj, id, 1);
    for (int i = 5, *p = (int *)self + 5; i < 10; i++, p++) {
        if (p[2] != 0) {
            func_088D9CFC(p[2]);
        }
    }
    ((int *)self)[0x50 / 4] = 0;
}

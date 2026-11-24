/* Calls func_088D9F5C(self->w14, 0x17, 1) then func_088D9F5C(self->w10, table[self->w54].s2, 1). */
extern void func_088D9F5C(int, int, int);
extern unsigned short D_089CF1B8[];
void func_08930774(int *self) {
    func_088D9F5C(self[0x14 / 4], 0x17, 1);
    func_088D9F5C(self[0x10 / 4], D_089CF1B8[self[0x54 / 4] * 2 + 1], 1);
}

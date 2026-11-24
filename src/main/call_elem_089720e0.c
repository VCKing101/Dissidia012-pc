/* Calls func_08971810(self->items[i], self->w28, a2, a3, a4). */
extern void func_08971810(int, int, int, int, int);
void func_089720E0(int *self, int i, int a2, int a3, int a4) {
    func_08971810(((int *)self[0])[i], self[0x28 / 4], a2, a3, a4);
}

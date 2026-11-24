/* Calls func_089716D4(self->items[i], self->w28, a2). */
extern void func_089716D4(int, int, int);
void func_08972080(int *self, int i, int a2) {
    func_089716D4(((int *)self[0])[i], self[0x28 / 4], a2);
}

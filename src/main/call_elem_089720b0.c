/* Calls func_08971768(self->items[i], self->w28, a2). */
extern void func_08971768(int, int, int);
void func_089720B0(int *self, int i, int a2) {
    func_08971768(((int *)self[0])[i], self[0x28 / 4], a2);
}

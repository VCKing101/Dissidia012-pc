/* Store the definition id, look the definition up and run both setup steps from its fields, then flag active. */
extern int func_088BB88C(int id);
extern void func_08930858(int *self, int a, int b, int c, unsigned char v);
extern void func_089309D4(int *self, unsigned char v);
void func_089303E4(int *self, int a1, int a2, int a3, int id)
{
    int d;
    self[0x13] = id;
    d = func_088BB88C(id);
    if (d == 0) {
        return;
    }
    func_08930858(self, a1, a2, a3, *(unsigned char *)(d + 0xF));
    func_089309D4(self, *(unsigned char *)(d + 0xC));
    self[0] = 1;
}

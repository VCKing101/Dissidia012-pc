/* Looks up two records by the id at +0x168 and, when both exist, applies the second's fields to the object at +0x38. */
extern int D_08A4FFF4;
extern int func_088CC364(int id);
extern int func_08A4EEC4(int a, int b);
extern void func_088DAE0C(int obj, int a, int b, int c, int d);
void func_08A5EAA8(int self) {
    int r = func_088CC364(*(int *)(self + 0x168));
    if (r == 0) {
        return;
    }
    r = func_08A4EEC4(D_08A4FFF4, *(unsigned short *)(r + 2));
    if (r == 0) {
        return;
    }
    func_088DAE0C(*(int *)(self + 0x38), *(int *)(r + 4), *(int *)(r + 8), -1, -1);
}

/* Flag the scene as active, queue its data load with the global handles and reset the three UI blocks. */
extern int D_089D7F90;
extern int D_089D83A8;
extern int func_08911664(int h);
extern void func_088FCC70(int a, int b, int size, int c, int d);
extern void func_088D7574(int p);
extern void func_088CDB04(int p);
extern void func_088CE7F4(int p);
void func_08916494(int self)
{
    int a;
    int b;
    int c;
    int size;
    *(char *)(self + 0x7E2) = 1;
    a = D_089D7F90;
    size = 0xC8000;
    c = *(int *)(self + 0x750);
    b = func_08911664(D_089D83A8);
    func_088FCC70(a, c, size, b, 0);
    func_088D7574(self + 0xA0);
    func_088CDB04(self + 0x3C0);
    func_088CE7F4(self + 0x404);
}

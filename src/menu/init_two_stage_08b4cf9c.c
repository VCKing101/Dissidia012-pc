/* Runs the two set-up routines with the long argument list and marks the object as set up (word 0 = 1). */
extern void func_08B4D084(int *self, int a1, int a2, int a3, unsigned char b4, unsigned char b5);
extern void func_08B4D248(int *self, int a, int b, int c, int d, int e, int f, int g, unsigned char h, unsigned char i, unsigned char j);
void func_08B4CF9C(int *self, int a1, int a2, int a3, unsigned char p4, unsigned char p5, int p6, int p7,
                   int p8, int p9, int p10, int p11, int p12, unsigned char p13, unsigned char p14)
{
    func_08B4D084(self, a1, a2, a3, p4, p5);
    func_08B4D248(self, p6, p7, p8, p9, p10, p11, p12, p13, p4, p14);
    self[0] = 1;
}

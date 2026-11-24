/* Run two setup stages, clear a field and flag the object active. */
extern void func_0892C924(int *self, int a1, int a2, int a3);
extern void func_0892CAB8(int *self, int p4, int p5, int p6, int p7, int p8);
void func_0892C868(int *self, int a1, int a2, int a3, int p4, int p5, int p6, int p7, int p8)
{
    func_0892C924(self, a1, a2, a3);
    func_0892CAB8(self, p4, p5, p6, p7, p8);
    self[9] = 0;
    self[0] = 1;
}

/* Runs three setup calls on the same pair of data blocks. */
extern char D_088909C8[];
extern char D_08890A00[];
extern void func_0899B6B0(void *a, void *b);
extern void func_0899B6F4(void *a, void *b);
extern void func_0899B728(void *a, void *b);

void func_08AFC424(void)
{
    func_0899B6B0(D_088909C8, D_08890A00);
    func_0899B6F4(D_088909C8, D_08890A00);
    func_0899B728(D_088909C8, D_08890A00);
}

/* Registers the pair of callbacks at 0x088909C8 and 0x08890A00 with three registry calls. */
extern void func_088909C8(void);
extern void func_08890A00(void);
extern void func_0899B6B0(void (*a)(void), void (*b)(void));
extern void func_0899B6F4(void (*a)(void), void (*b)(void));
extern void func_0899B728(void (*a)(void), void (*b)(void));
void func_08B125D8(void)
{
    func_0899B6B0(func_088909C8, func_08890A00);
    func_0899B6F4(func_088909C8, func_08890A00);
    func_0899B728(func_088909C8, func_08890A00);
}

typedef struct Channel { char data[0x240]; } Channel;
extern Channel D_09B47B50[];
extern void func_08873A64(Channel *c);
void func_08873F54(void)
{
    func_08873A64(&D_09B47B50[1]);
}

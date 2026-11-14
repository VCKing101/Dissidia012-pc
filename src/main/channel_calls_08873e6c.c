typedef struct Channel { char data[0x240]; } Channel;
extern Channel D_09B47B50[];
extern void func_088739CC(Channel *c);
extern void func_08873A64(Channel *c);
void func_08873E6C(void)
{
    func_088739CC(&D_09B47B50[2]);
}
void func_08873E90(void)
{
    func_08873A64(&D_09B47B50[2]);
}

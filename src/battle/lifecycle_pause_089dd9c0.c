extern void func_089E6C3C(void *);
extern void func_089E813C(void *);
extern void func_089ED2F4(void *, int);
extern void func_089E6C74(void *);
extern void func_089E81B0(void *);
extern void func_089ED28C(void *, int);

int func_089DD9C0(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E6C3C(object + 0xA8C);
    func_089E813C(object + 0x1DC);
    func_089ED2F4(object + 0x1D04, 1);
    return 0;
}

int func_089DDA10(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E6C74(object + 0xA8C);
    func_089E81B0(object + 0x1DC);
    func_089ED28C(object + 0x1D04, 1);
    return 0;
}

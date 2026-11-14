extern void func_089DDE4C(void *);
extern void func_089E6CAC(void *, void *, int);
extern void func_089E0BE8(void *, void *, int);

int func_089DDDAC(unsigned char *object, void *first, int first_count, void *second, int second_count)
{
    func_089DDE4C(object);
    if (!first || first_count <= 0 || !second || second_count <= 0) return -1;
    func_089E6CAC(object + 0xA8C, first, first_count);
    func_089E0BE8(object + 0x1C6C, second, second_count);
    return 0;
}

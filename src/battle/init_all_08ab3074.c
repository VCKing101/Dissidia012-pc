/* Initialises the unit's sub-objects in order; returns 0 on success or a negative code naming the failing stage. */
extern void func_08AB3014(void *p);
extern int func_08A9727C(char *p, int arg);
extern int func_08A824D8(char *a, char *b, char *c, char *d);
extern int func_08AAE248(char *a, char *b, char *c, char *d);
extern int func_08A64F60(char *a, char *b);
extern int func_08A7B2A4(char *a, char *b, char *c, char *d, char *e);
extern int func_08A7EA90(char *a, char *b, char *c);
extern int func_08A86DEC(char *a, int arg, char *b);
int func_08AB3074(unsigned char *self, int arg)
{
    char *p4;
    char *p34c;
    char *p390;
    func_08AB3014(self);
    if (arg == 0) {
        return -1;
    }
    p4 = (char *)self + 4;
    if (func_08A9727C(p4, arg) != 0) {
        return -2;
    }
    p34c = (char *)self + 0x34C;
    if (func_08A824D8((char *)self + 0x198, p4, p34c, (char *)self + 0x440) != 0) {
        return -3;
    }
    p390 = (char *)self + 0x390;
    if (func_08AAE248(p34c, p4, p390, (char *)self + 0x36C) != 0) {
        return -4;
    }
    if (func_08A64F60((char *)self + 0x360, p4) != 0) {
        return -5;
    }
    if (func_08A7B2A4((char *)self + 0x36C, p4, p34c, (char *)self + 0x360, p390) != 0) {
        return -6;
    }
    if (func_08A7EA90(p390, p4, p34c) != 0) {
        return -7;
    }
    if (func_08A86DEC((char *)self + 0x440, arg, p4) != 0) {
        return -8;
    }
    *self = 1;
    return 0;
}

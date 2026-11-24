/* Creates the main task's 0x800-byte stack, initialises its sub-object, sets two flags and registers its entry function. */
struct Task { int stack; char sub[0x28]; int a; int b; };
extern struct Task *D_08B491E8;
extern const char D_08B491BC[];
extern int func_088820D8(int);
extern void func_0886504C(void *, int, int);
extern void func_08B48A50(struct Task *);
extern void func_08B489BC(void);
extern void func_08865088(void *, int, int, void (*)(void), const char *);
void func_08B48938(void)
{
    D_08B491E8->stack = func_088820D8(0x800);
    func_0886504C(D_08B491E8->sub, D_08B491E8->stack, 0x800);
    D_08B491E8->a = 1;
    D_08B491E8->b = 1;
    func_08B48A50(D_08B491E8);
    func_08865088(D_08B491E8->sub, 1, 1, func_08B489BC, D_08B491BC);
}

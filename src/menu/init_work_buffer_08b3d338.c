/* Allocates the 0x2000-byte work buffer for the current menu, sets it up and starts the DummyTask. */
typedef struct { char d[0x58]; int buf; char t[0x28]; int one; int two; } Cur;
extern Cur *D_08B41CC0;
extern int func_088820D8(int);
extern void func_0886504C(void *, int, int);
extern void func_08B3569C(Cur *);
extern void func_08865088(void *, int, int, void (*)(void), const char *);
extern void func_08B3D40C(void);
extern const char D_08B4190C[];
void func_08B3D338(void)
{
    D_08B41CC0->buf = func_088820D8(0x2000);
    func_0886504C((char *)D_08B41CC0 + 0x5C, D_08B41CC0->buf, 0x2000);
    D_08B41CC0->one = 1;
    D_08B41CC0->two = 1;
    func_08B3569C(D_08B41CC0);
    func_08865088((char *)D_08B41CC0 + 0x5C, -1, 0x4B0, func_08B3D40C, D_08B4190C);
}

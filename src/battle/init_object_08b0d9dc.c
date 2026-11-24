// Constructor: clears the head word, initialises the members at +4, +8, the two-element 0x10-byte array at +0x6C and the member at +0xA0, clears the other fields; returns self.
extern void func_08B0D0E0(void *);
extern void func_08B143B0(void *);
extern void func_08B12060(void *);
extern void func_08B0D7DC(void);
extern void func_08993560(void *, int, int, void (*)(void));
typedef struct {
    int w0;
    char m4[4];
    char m8[0x6C - 8];
    char arr[0x20];
    int w8c, w90, w94;
    unsigned char b98, b99, b9a, b9b;
    int w9c;
    char ma0[1];
} Obj08B0D9DC;
Obj08B0D9DC *func_08B0D9DC(Obj08B0D9DC *self)
{
    self->w0 = 0;
    func_08B0D0E0(self->m4);
    func_08B143B0(self->m8);
    func_08993560(self->arr, 2, 0x10, func_08B0D7DC);
    self->w8c = 0;
    self->w90 = 0;
    self->w94 = 0;
    int one = 1;
    self->b9a = 0;
    self->b9b = 0;
    self->w9c = one;
    func_08B12060(self->ma0);
    self->b98 = 0;
    self->b99 = 0;
    return self;
}

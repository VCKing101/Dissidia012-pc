/* Marks the block ready, formats the message text with func_0893476C and func_0887DF14, and opens the message window through func_08AF4CDC; then runs func_088D9CFC on +0x484. */
typedef struct {
    char p0[0x484]; int w484;
    char p1[0x1AF6 - 0x488]; char text[0x1B88 - 0x1AF6];
    int w1B88; int w1B90a; int w1B90; char p2[0x1C98 - 0x1B94]; int w1C98; int w1C9C; int w1CA0;
} Blk;
typedef struct { char p0[0x46AC]; Blk *blk; } Obj;
extern int D_089D6998;
extern int D_08B01CF8;
extern unsigned short D_08ADFBF4[];
extern char D_08ADFB34[];
extern int func_0887AFBC(int a, int b, int c);
extern void func_0893476C(void *dst, const char *fmt, int a);
extern void func_0887DF14(int a, void *dst, int n, void *src);
extern void func_08AF4CDC(int a, int b, void *c, int d, void *e, int f, int g, int h, int i, int j);
extern void func_088D9CFC(int a);
void func_08ADC434(Obj *self) {
    int local_60[2];
    char buf[64];
    int h;
    char *txt;
    self->blk->w1B90 = 1;
    self->blk->w1B88 = 0;
    local_60[0] = func_0887AFBC(0x09B49118, self->blk->w1CA0, 0);
    func_0893476C(buf, D_08ADFB34, self->blk->w1C9C);

    txt = self->blk->text;
    func_0887DF14(D_089D6998, txt, 0x80, buf);
    local_60[1] = (int)self->blk->text;
    func_08AF4CDC(D_08B01CF8, 1, self->blk, D_08ADFBF4[self->blk->w1C98], local_60, 2, 1, 0, 0, 0);
    func_088D9CFC(self->blk->w484);
}

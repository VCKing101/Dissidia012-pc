typedef struct Win { char p0[0x88]; char c88; char p1[0x488-0x89]; char c488; char p2[0x6F8-0x489]; int f6F8; char p3[0xA88-0x6FC]; int fA88; char cA8C; char p4[0xAE4-0xA8D]; int fAE4; char p5[0xAFC-0xAE8]; int fAFC; int fB00; int fB04; int fB08; } Win;
typedef struct Ui { char pad[0x14C]; Win *w; } Ui;
extern int D_089D712C;
extern int D_089D6998;
extern void func_088D3CF0(void *, void *, void *);
extern void func_088D9F5C(int, int, int);
extern int func_088F2D8C(int, int);
extern int *func_088F2DC0(int, int);
extern void func_088ADFCC(void *);
extern int func_088D6254(int);
extern void *func_0887B500(int, int, int);
extern int func_089EF1E8(void *, int, int, void *, int);
extern void func_088AE048(int, void *, void *, void *, void *, void *);
extern void func_0893476C(void *, void *, ...);
extern void func_0887DF14(int, void *, int, void *);
extern int func_0887D868(int, int, void *, int, int, int, int, int);
extern void func_088D428C(void *, void *, void *, int);
extern void func_089F3F68(void *, int);
extern unsigned char D_08A09D42[], D_08A0AC58[], D_08A0AC6C[], D_08A09D6A[];

void func_089F4A4C(Ui *self, int mode)
{
    int local0;
    void *local4;
    int x, y, z, w, v;
    char buf[0x80];
    char fmt[0x40];
    int count;
    int arg2;
    int r;
    unsigned short idx;
    func_088D3CF0(&self->w->c88, &self->w->c488, D_08A09D42);
    count = 1;
    if (mode == 5) {
        func_088D9F5C(self->w->f6F8, 10, 1);
        count = 2;
    }
    local0 = 0;
    {int r2 = func_088F2D8C(D_089D712C + 0x1E208, count);
    if (r2 != 0) {
        int *e = func_088F2DC0(D_089D712C + 0x1E208, count);
        arg2 = e[1];
        local0 = e[0];
        r = e[2];
    } else {
        arg2 = 1;
        func_088ADFCC(&local0);
        r = 0;
    }}
    idx = func_088D6254(arg2);
    if (idx != 0xFFFF) {
        local4 = func_0887B500(self->w->fA88, 1, idx);
    } else {
        local4 = 0;
    }
    self->w->fAE4 = func_089EF1E8(self, 0, 0x74, &local4, 1);
    func_088AE048(local0, &x, &y, &z, &w, &v);
    func_0893476C(fmt, D_08A0AC58, x, y, z);
    func_0887DF14(D_089D6998, buf, 0x80, fmt);
    self->w->fB00 = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    func_0893476C(fmt, D_08A0AC6C, r);
    func_0887DF14(D_089D6998, buf, 0x80, fmt);
    local4 = buf;
    self->w->fB08 = func_089EF1E8(self, 0, 0x9E, &local4, 1);
    self->w->fAFC = func_089EF1E8(self, 0, 0x75, 0, 0);
    self->w->fB04 = func_089EF1E8(self, 0, 0x76, 0, 0);
    func_088D428C(&self->w->c488, &self->w->cA8C, D_08A09D6A, 1);
    func_089F3F68(self, 0xA5);
    func_089F3F68(self, 0xAA);
}

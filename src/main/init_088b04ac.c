typedef struct S { char p0[8]; int f8; int w; int f10; int f14; } S;
extern void func_08935828(void *, int, int);
extern void func_088AEFA0(S *);
extern void func_08935BB4(void *, int, int);
extern void func_08935524(void *, int, int);
extern int func_089359E8(void *);
extern void func_0893476C(void *, void *, int);
extern void func_088ADB10(int, void *);
extern void func_088ADF68(int, void *, int, int);
extern char D_089CB700[];
extern int D_089D712C;
extern unsigned long long D_089CB6F0;

#define W(t, o) (*(t *)(self->w + (o)))

void func_088B04AC(S *self, int w, int a2, int a3, int a4)
{
    int i;
    int off;
    int p;
    int n;
    int tmp;
    self->f8 = a2;
    self->w = w;
    self->f10 = a3;
    self->f14 = a4;
    func_08935828((void *)(w + 0x7E4), 0, 0x600);
    func_08935828((void *)(self->w + 0xDE4), 0, 0x14);
    func_088AEFA0(self);
    W(int, 0xDF4) = self->w + 0x2AB0;
    W(int, 0xDB4) = self->w + 0xDF8;
    W(int, 0xDB8) = self->w + 0xE0C;
    W(int, 0xDBC) = self->w + 0xE4C;
    W(int, 0xDA8) = self->w + 0xDE4;
    W(int, 0x85C) = W(int, 0x2B88);
    W(int, 0x860) = W(int, 0x2B88);
    W(int, 0x858) = self->f10;
    func_08935BB4((void *)(self->w + 0x848), W(int, 0x2B8C), 0xD);
    func_08935524((void *)(self->w + 0xDC0), W(int, 0x2B94), 0x10);
    W(char, 0x830) = 0;
    func_08935828((void *)self->w, 0, 0x7E4);
    func_08935828((void *)(self->w + 0xE68), 0, 0xC);
    func_08935828((void *)(self->w + 0xE74), 0, 0x1C20);
    W(int, 0xDD8) = self->w + 0xE68;
    W(int, 0xE68) = 100;
    W(int, 0xE70) = self->w + 0xE74;
    W(char, 0x2B54) = 0;
    i = 0;
    p = self->w;
    off = 0;
    do {
        p = p;
        func_08935BB4((void *)(self->w + off), W(int, 0x2B90), 0x14);
        n = func_089359E8((void *)(self->w + off));
        func_0893476C((void *)(self->w + off + n), D_089CB700, i);
        i++;
        off += 0x14;
    } while (i < 100);
    *(char *)(self->w + 2000) = 0;
    p = self->w;
    *(int *)(p + 0x844) = p;
    func_08935BB4((void *)(p + 0x2B40), *(int *)(p + 0x2B90), 0x14);
    n = func_089359E8((void *)(self->w + 0x2B40));
    *(char *)(self->w + n + 0x2B40) = 0x2A;
    *(char *)(self->w + n + 0x2B41) = 0;
    func_088ADB10(D_089D712C, (void *)(self->w + 0x7E4));
    func_088ADF68(D_089D712C, (void *)(self->w + 0xD68), 0, 0);
    func_088ADF68(D_089D712C, W(void *, 0xDA8), 0, 0);
    W(int, 0x860) = W(int, 0x2B88);
    W(int, 0x2B64) = 0;
    W(int, 0x2B6C) = 0;
    W(char, 0x2B7F) = 0;
    W(int, 0x2B74) = 0;
    W(int, 0x2B78) = 0;
    W(int, 0x2B70) = 0;
    W(unsigned long long, 0x2B58) = D_089CB6F0;
    W(char, 0x2B80) = 0;
}

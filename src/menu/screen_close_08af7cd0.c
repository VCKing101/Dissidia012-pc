/* Closes the screen: tears down the owned object and its sub-allocation when present, and clears the open flag. */
typedef struct Sub { char pad0[0x10]; int f10; char pad1[0x89C - 0x14]; int h89C; } Sub;
typedef struct Own { char pad0[0x138]; int heap; int pad13C; Sub *sub; } Own;
extern unsigned char D_09B6CDA0[];
extern int D_089D6998;
extern void func_0885C538(int, void *, int);
extern void func_0887B1F4(int, int);
extern void func_0887E684(int);
extern void func_08AD10EC(void);
extern void func_08AF2400(Own *);
extern void func_08AF7BA8(Own *);
extern void func_08AF7C74(Own *);
void func_08AF7CD0(Own *self) {
    if (self->heap != 0) {
        int h;
        func_08AF7BA8(self);
        func_08AD10EC();
        h = self->sub->h89C;
        if (h != 0) {
            if (h != 0) {
                func_0887B1F4(h, 3);
                self->sub->h89C = 0;
            }
        }
        func_08AF7C74(self);
        func_0887E684(D_089D6998);
        func_0885C538(self->heap, (void *)self->sub->f10, 0);
        self->sub->f10 = 0;
        func_0885C538(self->heap, self->sub, 0);
        self->sub = 0;
        func_08AF2400(self);
    } else {
        self->sub = 0;
    }
    D_09B6CDA0[0xE6] = 0;
}

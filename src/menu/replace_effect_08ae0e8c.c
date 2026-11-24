/* Frees the effect handle in slot idx of self->p850+0x550, creates a new one with func_08AF3A50(D_08B01CF8, 0, 0, a2, 0, 0), starts it and refreshes the display. */
typedef struct { char p0[0x550]; int h[1]; } Blk;
typedef struct { char p0[0x850]; Blk *p850; } Obj;
typedef struct { short a; short b; int c; int d; } Rec;
extern int D_08B01CF8;
extern Rec *func_08AE4708(void);
extern void func_0887BA5C(void);
extern int func_08AF3A50(int a, int b, int c, int d, int e, int f);
extern void func_0887B924(int h);
extern void func_08AE0AB4(Obj *o, int idx, int v);
extern void func_08AE0F70(Obj *o, int a, int idx);
void func_08AE0E8C(Obj *self, int idx, int a2) {
    int pad0[1];
    int sv[1];
    int pad1[2];
    int off;
    Blk *b;
    sv[0] = ((short *)((char *)func_08AE4708() + idx * 12))[1];
    b = self->p850;
    off = idx * 4;
    if (*(int *)((char *)b + off + 0x550) != 0) {
        func_0887BA5C();
        *(int *)((char *)self->p850 + off + 0x550) = 0;
    }
    *(int *)((char *)self->p850 + off + 0x550) = func_08AF3A50(D_08B01CF8, 0, 0, a2, 0, 0);
    func_0887B924(*(int *)((char *)self->p850 + off + 0x550));
    func_08AE0AB4(self, idx, sv[0]);
    func_08AE0F70(self, 0, idx);
}

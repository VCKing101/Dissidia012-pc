typedef struct Menu102 {
    unsigned flags;
    char pad0[0xA5C - 4];
    char blk_a5c[0x58];
    int hab4;
    int hab8;
    char pad1[0xEDC - 0xABC];
    int hedc;
    int hee0;
    char pad2[0xEF0 - 0xEE4];
    char blk_ef0[0x40];
    int hf30;
    int hf34;
    int hf38;
    int hf3c;
    int hf40[8];
    char pad3[0x10A4 - 0xF60];
    int sel10a4;
    char pad4[0x10EC - 0x10A8];
    int idx10ec;
    int w10f0;
    int w10f4;
} Menu102;
typedef struct Ent102 { char p[6]; short cnt; short pad; short ids[8]; short pad2; } Ent102;
typedef struct Root102 { char pad[0x30]; Menu102 *m; } Root102;
extern void func_0887BA5C(int h);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern int func_0887E534(int h);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_0887B944(int h);
extern void func_088D9CFC(int h);
extern void func_088D9CD8(int h);
extern char D_089CCFD4[];

void func_088FD314(Root102 *self) {
    Menu102 *m;
    Ent102 *ent;
    int v;
    ent = (Ent102 *)((char *)self->m + 0x1A) + self->m->sel10a4;
    if (self->m->hf30 != 0) {
        func_0887BA5C(self->m->hf30);
        self->m->hf30 = 0;
    }
    v = func_0887B3EC(self->m->hedc, self->m->hee0, 0, ent->ids[self->m->idx10ec], 0, 0);
    self->m->hf30 = v;
    if (self->m->hf34 != 0) {
        func_0887BA5C(self->m->hf34);
        self->m->hf34 = 0;
    }
    if (ent->cnt >= 2) {
        v = func_0887E534(self->m->hf40[self->m->idx10ec]);
        self->m->hf34 = v;
    }
    func_088D428C(self->m->blk_a5c, self->m->blk_ef0, D_089CCFD4, 1);
    self->m->flags = self->m->flags | 8;
    if (ent->cnt < 2) {
        func_0887B944(self->m->hf3c);
        func_088D9CFC(self->m->hab4);
        func_088D9CFC(self->m->hab8);
    } else if (self->m->idx10ec == 0) {
        func_088D9CFC(self->m->hab4);
        func_088D9CD8(self->m->hab8);
    } else if (self->m->idx10ec == self->m->w10f4) {
        func_088D9CFC(self->m->hab8);
        func_088D9CD8(self->m->hab4);
    } else {
        func_088D9CD8(self->m->hab4);
        func_088D9CD8(self->m->hab8);
    }
}

typedef struct E7 { int key; int pad4; unsigned flags; } E7;
typedef struct O7 { char pad0[0xC]; int h; char pad1[0xF8]; E7 *ents; } O7;
typedef union W7 { int w; char b[4]; } W7;
extern int func_089B0E3C(int h);
extern int func_089B2CCC(int h, int i, int j);
extern int *func_0886CDE0(O7 *o, int i);
extern int *func_0886CE28(O7 *o, int i);
extern void func_089B4918(int *p, int v);
extern void func_089B4948(int *p, int v);
extern int func_089AD32C(int h);

void func_0886C050(O7 *self, int idx, int val, unsigned char rec) {
    if (rec != 0) {
        int count = func_089B0E3C(self->h);
        int i;
        for (i = idx; i < count; i++) {
            if (func_089B2CCC(self->h, i, idx) != 0) {
                func_0886C050(self, i, val, 0);
            }
        }
    } else {
        E7 *e = &self->ents[idx];
        unsigned bit = 0x10000;
        int *p;
        W7 u;
        if ((e->flags & bit) != 0 && e->key == val) {
            return;
        }
        e->key = val;
        p = func_0886CDE0(self, idx);
        if (p == 0) {
            return;
        }
        u.w = *p;
        if (u.b[1] != 0) {
            
func_089B4918(p + u.b[1], val);
        } else {
        }
        if (self->ents[idx].flags & bit) {
            return;
        }
        p = func_0886CE28(self, idx);
        if (p == 0) {
            return;
        }
        u.w = *p;
        if (u.b[1] != 0) {
            
func_089B4948(p + u.b[1], func_089AD32C(self->h));
        } else {
        }
        self->ents[idx].flags |= bit;
    }
}

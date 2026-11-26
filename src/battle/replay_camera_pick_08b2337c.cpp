/* Looks up the replay camera data at the play frame and starts the camera (falls back to the main entry). */
struct Obj { char p0[4]; void *rep; char p1[0x178 - 8]; int i178; };
struct Ent { char p0[0x20]; unsigned char b20; };
extern "C" {
extern float D_089D5EAC;
extern Ent *func_088CA240(void *r);
extern Ent *func_088CA2E0(void *r);
extern Ent *func_088CA354(void *r);
extern Ent *func_088CA3C8(void *r);
extern void func_08B253FC(Obj *o, Ent *a, Ent *b, Ent *c, Ent *d);
extern void func_08B23E3C(Obj *o, int a, int b);
extern void func_08B232FC(Obj *o, int a);
extern void func_08B2337C(Obj *self) {
    Ent *prev = func_088CA3C8(self->rep);
    Ent *cur = func_088CA240(self->rep);
    Ent *next = func_088CA2E0(self->rep);
    Ent *after = func_088CA354(self->rep);
    do {
    if (cur != 0) {
        if (((int)prev | (int)next | (int)after) == 0) {
            func_08B253FC(self, cur, cur, cur, cur);
            func_08B23E3C(self, 1, cur->b20);
            func_08B232FC(self, 0);
            break;
        }
    } else {
        self->i178 = 0;
        func_08B23E3C(self, 0, 8);
        func_08B232FC(self, 0);
        break;
    }
    if (prev == 0) prev = cur;
    if (next == 0) next = cur;
    if (after == 0) after = next;
    if (D_089D5EAC != 0.0f) {
        func_08B253FC(self, prev, cur, next, after);
        func_08B23E3C(self, 1, cur->b20);
        func_08B232FC(self, 0);
    }
    } while (0);
}
}

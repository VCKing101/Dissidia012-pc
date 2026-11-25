struct Mgr { char p[0x7A4]; int f7a4; };
struct Ent { char p[0x14]; short s14; };
struct Node { char p[0x52C]; Ent *e; };
struct St { unsigned char b0; char pad[3]; int i4; int i8; char pad2[0x10]; int i1c; };
struct Obj {
    char p0[8];
    int f8;
    char pc;
    unsigned char bd;
    char pe[2];
    float f10;
    int f14;
    int f18;
    char p1c[0x28];
    char f44[0x100];
    char pad[0x148 - 0x144];
    int f148;
    int f14c;
    int f150;
};
extern "C" {
extern Mgr *D_089D6A10;
extern int D_08A62280;
Node *func_08A22C78(int);
int func_08891D84(int, void *, void *, int, int, int);
int func_089E99C0(int, int);
void func_089F84E0(void *);
void func_089F85B0(void *, int, int);
void func_08A62020(int, int, float);
void func_089EF644(void *);
void func_089DDB50(int, int, int);

int func_089F0720(Obj *self) {
    if (self->bd == 1) {
        int s2 = (self->f148 + self->f14c) % 2;
        int s3 = s2;
        {
            Node *n = func_08A22C78(s3);
            if (n == 0) return -1;
            int s4 = n->e->s14;
            St st;
            st.b0 = 0;
            st.i1c = 0;
            if (func_08891D84(self->f150, &st, &st.i4, 0, 0, 0) != 0) return -2;
            if (st.b0 == 0) {
                void *s5;
                s3 = func_089E99C0(s3, s4);
                s5 = self->f44;
                func_089F84E0(s5);
                func_089F85B0(s5, s4, s3);
                self->f8 = 4;
                self->f14 = s4;
                self->f18 = -st.i8;
            } else {
                self->f8 = 8;
            }
            if (st.b0 == 0 && self->f14 < self->f18 && (unsigned char)((st.i1c & 4) != 0) == 1) {
                func_08A62020(D_08A62280, 3, 0.0f);
            }
            func_089EF644((char *)self + 0x1C);
            if (D_089D6A10->f7a4 == 0) return -3;
            func_089DDB50(D_089D6A10->f7a4, (s2 + 1) % 2, 1);
            self->f10 = 0.0f;
        }
    }
    return 0;
}
}

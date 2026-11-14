extern "C" {
void func_08810D8C(void *self);
void func_08992F04(void *p);
extern char D_089C6DB0[];
extern char D_089C6DC0[];
extern char D_089C6DD0[];
extern char D_089C6DE0[];
extern char D_089C6DF0[];
extern char D_089C6E00[];
}

/* Member object: two data words, then its vptr. */
struct Part08810CAC {
    int field_00;
    int field_04;
    void *vtable;

    static void destroy(Part08810CAC *p, int flags, void *vt) {
        if (p != 0) {
            p->vtable = vt;
            if (flags & 1) {
                func_08992F04(p);
            }
        }
    }
};

struct Owner08810CAC {
    int field_00;
    int field_04;
    int field_08;
    Part08810CAC part_0C;
    Part08810CAC part_18;
    Part08810CAC part_24;
    Part08810CAC part_30;
    Part08810CAC part_3C;
    void *vtable;

    static void destroy(Owner08810CAC *self, int flags) {
        if (self != 0) {
            self->vtable = D_089C6DB0;
            func_08810D8C(self);
            Part08810CAC::destroy(&self->part_3C, 0, D_089C6E00);
            Part08810CAC::destroy(&self->part_30, 0, D_089C6DF0);
            Part08810CAC::destroy(&self->part_24, 0, D_089C6DE0);
            Part08810CAC::destroy(&self->part_18, 0, D_089C6DD0);
            Part08810CAC::destroy(&self->part_0C, 0, D_089C6DC0);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08810CAC(Owner08810CAC *self, int flags) {
    Owner08810CAC::destroy(self, flags);
}
}

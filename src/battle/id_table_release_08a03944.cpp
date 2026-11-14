extern "C" {
extern void func_08A04624(void *, void *);
extern void func_08A03910(void *);
}

struct IdLink {
    int id;
    void *target;
};
struct IdTable3944 {
    unsigned char field_00;
    char pad_001[3];
    int field_04;
    IdLink entries[20];
};

extern "C" {
void func_08A03944(IdTable3944 *self)
{
    for (int i = 0; i < 20; i++) {
        IdLink *e = &self->entries[i];
        if (e->target) {
            func_08A04624(self, e->target);
        }
    }
    func_08A03910(self);
}
}

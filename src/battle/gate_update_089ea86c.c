extern void func_089EAE18(void *);
extern void func_089EAEEC(void *);
extern void func_089EB0C8(void *);
extern void func_089EAFB0(void *);
extern void func_089EB22C(void *);
extern void func_089EB3E8(void *, int, int);
extern void func_089EB54C(void *);

typedef struct GateA86C {
    unsigned char field_00;
} GateA86C;

int func_089EA86C(GateA86C *self, int arg1, int arg2)
{
    if (!self->field_00) {
        return -1;
    }
    func_089EAE18(self);
    func_089EAEEC(self);
    func_089EB0C8(self);
    func_089EAFB0(self);
    func_089EB22C(self);
    func_089EB3E8(self, arg1, arg2);
    func_089EB54C(self);
    return 0;
}

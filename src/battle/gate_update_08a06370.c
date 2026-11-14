extern void func_08A06524(void *);
extern void func_08A06684(void *);
extern void func_08A0676C(void *);

typedef struct Gate6370 {
    unsigned char field_00;
} Gate6370;

int func_08A06370(Gate6370 *self)
{
    if (!self->field_00) {
        return -1;
    }
    func_08A06524(self);
    func_08A06684(self);
    func_08A0676C(self);
    return 0;
}

extern void func_08A17A00(void *);
extern void func_089F83AC(void *);
extern void func_08A17018(void *);
extern void func_08A08334(void *);

typedef struct Gate7808 {
    unsigned char field_00;
    char pad_001[0x17];
    char field_18[0x38];
    char field_50[0x88];
    char field_D8[4];
} Gate7808;

int func_08A17808(Gate7808 *self)
{
    if (!self->field_00) {
        return -1;
    }
    func_08A17A00(self);
    func_089F83AC(self->field_18);
    func_08A17018(self->field_50);
    func_08A08334(self->field_D8);
    return 0;
}

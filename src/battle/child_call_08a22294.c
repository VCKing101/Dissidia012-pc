typedef struct ChildCall22294 {
    unsigned char active;
    char pad_001[0xFF];
    char field_100[0x4];
} ChildCall22294;

extern void func_08A07148(void *sub);

int func_08A22294(ChildCall22294 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A07148(self->field_100);
    return 0;
}

typedef struct PairSetup {
    unsigned char active;
    char pad_01[0x7];
    char field_08[0x128];
    unsigned int kind;
    void *data;
} PairSetup;

extern void func_089F34D4(PairSetup *self);
extern void func_089F7D74(void *sub, void *first, void *second);

int func_089F351C(PairSetup *self, unsigned int kind, void *data, void *first, void *second)
{
    func_089F34D4(self);
    if (kind >= 2 || data == 0 || first == 0 || second == 0) {
        return -1;
    }
    func_089F7D74(self->field_08, first, second);
    self->kind = kind;
    self->data = data;
    self->active = 1;
    return 0;
}

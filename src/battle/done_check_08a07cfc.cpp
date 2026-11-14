typedef struct DoneCheck {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x4];
    int index;
    unsigned char done;
    char pad_11[0xB];
    char *entries;
    char pad_20[0x8];
    char field_28[0x4];
} DoneCheck;

extern "C" {
extern int func_08A156C0(void *sub);
extern void func_0889199C(void *entry);
}

struct DoneOps {
    static unsigned char finished(DoneCheck *self)
    {
        int result = 0;
        if (func_08A156C0(self->field_28) == 0 || self->field_04 == 1) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

int func_08A07CFC(DoneCheck *self)
{
    unsigned char done = DoneOps::finished(self);
    if (!done && self->done == 1) {
        func_0889199C(self->entries + self->index * 0x28);
    }
    self->done = done;
    return 0;
}

}

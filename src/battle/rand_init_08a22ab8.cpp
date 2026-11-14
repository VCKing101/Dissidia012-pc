extern "C" void func_08A22C60(void);
struct RandSource {
    char pad_000[0x4];
    int field_04;
    void *field_08;
    static void bind(RandSource *self, void *callback, int count)
    {
        self->field_08 = callback;
        self->field_04 = count;
    }
};
extern "C" {
void func_08A22B50(RandSource *self);
RandSource *func_08A22AB8(RandSource *self)
{
    RandSource::bind(self, (void *)func_08A22C60, 1);
    func_08A22B50(self);
    return self;
}
}

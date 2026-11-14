struct LookupCtx {
    char pad_000[0x88C];
    int *field_88C;
};

struct LookupOwner {
    char pad_000[0x138];
    LookupCtx *field_138;

    static bool is_none(unsigned short id)
    {
        return id == 0xFFFF;
    }
};

extern "C" {

int func_0887B3EC(int *table, int a, int b, unsigned short c, int d, int e);

int func_08A2B7D4(LookupOwner *self, unsigned short key)
{
    if (!LookupOwner::is_none(key)) {
        return func_0887B3EC(self->field_138->field_88C, 7, 0, key, 0, 0);
    }
    return 0;
}

}

typedef struct QueryCtx {
    char pad_000[0x41C];
    unsigned int field_41C;
} QueryCtx;

typedef struct QueryOwner {
    char pad_000[0x30C];
    QueryCtx *field_30C;
} QueryOwner;

extern unsigned char D_089D5F20;

int func_08A8A4F0(QueryOwner *self)
{
    int result = 0;
    if (D_089D5F20 == 1) {
        return 0;
    }
    if ((self->field_30C->field_41C & 0x8000) != 0) {
        result = 1;
    }
    return result;
}

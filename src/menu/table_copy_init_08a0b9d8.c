/* A 0x834-byte table copied whole into the object after its vtable. */
typedef struct CopyTable {
    int words[0x20D];
} CopyTable;

typedef struct CopyHolder {
    void *field_00;
    CopyTable field_04;
} CopyHolder;

extern int D_08A0BEC0;

CopyHolder *func_08A0B9D8(CopyHolder *self, CopyTable *source)
{
    self->field_00 = &D_08A0BEC0;
    self->field_04 = *source;
    return self;
}

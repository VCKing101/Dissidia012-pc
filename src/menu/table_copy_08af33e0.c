/* Copies the shared 0x778-byte table out. */
typedef struct CopyTable {
    int field_00[0x778 / 4];
} CopyTable;

typedef struct CopyShared {
    char pad_000[0x244];
    CopyTable field_244;
} CopyShared;

extern CopyShared D_09B6CDA0;

void func_08AF33E0(void *self, CopyTable *out)
{
    *out = D_09B6CDA0.field_244;
}

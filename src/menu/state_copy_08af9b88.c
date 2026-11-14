/* Sets the mode, refreshes and copies the 0x1FFC-byte state out. */
typedef struct CopyState2 {
    int field_00[0x1FFC / 4];
} CopyState2;

typedef struct CopyStateData {
    int field_00;
    char pad_004[0xC0C - 0x4];
    CopyState2 field_C0C;
} CopyStateData;

typedef struct CopyStateOwner {
    char pad_000[0x140];
    CopyStateData *field_140;
} CopyStateOwner;

extern void func_08AF3BE8(CopyStateOwner *self);

void func_08AF9B88(CopyStateOwner *self, int mode, CopyState2 *out)
{
    self->field_140->field_00 = mode;
    func_08AF3BE8(self);
    *out = self->field_140->field_C0C;
}

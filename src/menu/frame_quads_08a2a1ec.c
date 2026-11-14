/* Fills the three frame quads of row index; returns how many were written. */
typedef struct FrameQuad {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} FrameQuad;

typedef struct FrameState {
    char pad_000[0xC38];
    FrameQuad field_C38[8][4];
} FrameState;

typedef struct FrameOwner {
    char pad_000[0x138];
    FrameState *field_138;
} FrameOwner;

extern unsigned short D_08A2E722[];

int func_08A2A1EC(FrameOwner *self, int index)
{
    int count = 0;
    FrameQuad *quad;

    quad = &self->field_138->field_C38[index][count++];
    quad->field_00 = index + 0x23;
    quad->field_02 = 0x2A;
    quad->field_04 = index + 0x31;
    quad->field_06 = 0;
    quad = &self->field_138->field_C38[index][count++];
    quad->field_00 = index + 0x2A;
    quad->field_02 = 0x29;
    quad->field_04 = index + 0x31;
    quad->field_06 = 1;
    quad = &self->field_138->field_C38[index][count++];
    quad->field_00 = index + 0x31;
    quad->field_02 = 0x15;
    quad->field_04 = 0x38;
    quad->field_06 = D_08A2E722[index];
    return count;
}

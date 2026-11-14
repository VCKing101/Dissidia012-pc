/* Builds the two frame quads of row index at column x from the row table, then queues the rest. */
typedef struct FrameQuad {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} FrameQuad;

typedef struct FrameRow {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
    char pad_08[0x4];
} FrameRow;

extern void func_08A41454(void *list, FrameQuad *quads);
extern FrameRow D_08A38FA4[];
extern void *D_08A457EC;

void func_08A36160(void *self, FrameQuad *quads, int index, int x)
{
    quads[0].field_00 = x + 9;
    quads[0].field_02 = 0x14;
    quads[0].field_04 = index + 6;
    quads[0].field_06 = D_08A38FA4[index].field_02;
    quads[1].field_00 = x + 8;
    quads[1].field_02 = 0x14;
    quads[1].field_04 = index + 6;
    quads[1].field_06 = D_08A38FA4[index].field_00;
    func_08A41454(D_08A457EC, &quads[2]);
}

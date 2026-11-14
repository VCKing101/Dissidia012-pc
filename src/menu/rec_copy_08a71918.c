typedef struct PosPair {
    short field_00;
    short field_02;
} PosPair;

typedef struct PosSub {
    char pad_00[0x7A];
    PosPair field_7A[1];
} PosSub;

typedef struct PosGlobal {
    char pad_000[0x148];
    PosSub field_148;
} PosGlobal;

typedef struct PosSource {
    short field_00;
    short field_02;
    short field_04;
} PosSource;

extern PosGlobal D_09B6DF08;
extern void func_08A72044(int a, int b, int index);

void func_08A71918(int a, int b, PosSource *src)
{
    PosPair *dest = (PosPair *)((char *)&D_09B6DF08.field_148 + src->field_04 * 4 + 0x7A);
    short first = src->field_00;
    short second = src->field_02;
    dest->field_00 = first;
    dest->field_02 = second;
    func_08A72044(a, b, src->field_04);
}

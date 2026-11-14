typedef struct FrameInfo {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} FrameInfo;

extern FrameInfo D_08AE4780[];

short func_08AE46F0(int index)
{
    return D_08AE4780[index].field_04;
}

typedef struct FramePair {
    short field_00;
    short field_02;
} FramePair;

extern FramePair D_08AE4910[];

short func_08AE46C8(int index, int second)
{
    if (second == 0) {
        return D_08AE4910[index].field_00;
    } else {
        return D_08AE4910[index].field_02;
    }
}

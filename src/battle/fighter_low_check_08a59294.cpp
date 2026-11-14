struct HeightInfo {
    float value;
};

struct Fighter59294 {
    char pad_000[0x3A8];
    HeightInfo *info;
};

extern "C" {

extern float D_08A59354;

unsigned char func_08A59294(Fighter59294 *self)
{
    if (self == 0) {
        return 0;
    }
    int result = 0;
    if (self->info->value <= D_08A59354) {
        result = 1;
    }
    return result;
}

}

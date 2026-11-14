/* Clears the ten levels and both counters of each channel. */
struct Level08852550 {
    char pad_00[0x40];
    float value;
    char pad_44[0xC];
};

struct Channel08852550 {
    char pad_000[0x10];
    Level08852550 levels[10];
    int field_330;
    int field_334;
    char pad_338[8];
};

struct Pair08852550 {
    Channel08852550 channels[2];
};

extern "C" {

void func_08852550(Pair08852550 *p)
{
    for (int i = 0; i < 2; i++) {
        Channel08852550 *ch = &p->channels[i];
        for (int j = 0; j < 10; j++) {
            ch->levels[j].value = 0.0f;
        }
        ch->field_334 = 0;
        ch->field_330 = 0;
    }
}

}

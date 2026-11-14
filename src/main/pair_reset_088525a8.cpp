/* Clears the counters of the channel with the id (every channel for -1). */
struct Channel088525A8 {
    int id;
    char pad_004[0x32C];
    int field_330;
    int field_334;
    char pad_338[8];
};

struct Pair088525A8 {
    Channel088525A8 channels[2];
};

extern "C" {

void func_088525A8(Pair088525A8 *p, int id)
{
    for (int i = 0; i < 2; i++) {
        Channel088525A8 *ch = &p->channels[i];
        if (id != -1 && ch->id != id) {
            continue;
        }
        ch->field_334 = 0;
        ch->field_330 = 0;
    }
}

}

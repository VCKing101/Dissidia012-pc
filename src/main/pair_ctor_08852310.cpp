/* Constructs both 0x340-byte channels and marks them unused. */
struct Channel08852310 {
    int id;
    int field_04;
    int field_08;
    char pad_0C[0x334];
};

struct Pair08852310 {
    Channel08852310 channels[2];
};

extern "C" {

void func_08852308(void);
void func_08993560(void *array, int count, int size, void (*ctor)(void));

Pair08852310 *func_08852310(Pair08852310 *p)
{
    func_08993560(p, 2, 0x340, func_08852308);
    for (int i = 0; i < 2; i++) {
        Channel08852310 *ch = &p->channels[i];
        ch->id = -1;
        ch->field_04 = 0;
        ch->field_08 = 0;
    }
    return p;
}

}

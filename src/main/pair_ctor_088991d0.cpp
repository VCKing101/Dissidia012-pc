/* Constructs a channel pair: the main channel, one spare, no selection. */
struct Channel088991D0 {
    char pad_000[0x104];
    unsigned char active;
    char pad_105[3];
};

struct Pair088991D0 {
    Channel088991D0 main;
    Channel088991D0 spares[1];
    int selection;
};

extern "C" {

void func_08897244(Channel088991D0 *c);
void func_08993560(void *array, int count, int size, void (*ctor)(Channel088991D0 *));

Pair088991D0 *func_088991D0(Pair088991D0 *p)
{
    func_08897244(&p->main);
    func_08993560(p->spares, 1, sizeof(Channel088991D0), func_08897244);
    p->main.active = 0;
    p->selection = -1;
    for (int i = 0; i < 1; i++) {
        p->spares[i].active = 1;
    }
    return p;
}

}

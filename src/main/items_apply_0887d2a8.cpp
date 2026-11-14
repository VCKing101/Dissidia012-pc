extern "C" {
/* Applies every active item when the owner is ready: -1 otherwise. */
typedef struct Item0887D2A8 {
    unsigned char active;
    char pad_01[3];
    int a;
    int b;
    int field_0C;
    int field_10;
} Item0887D2A8;

typedef struct Owner0887D2A8 {
    int field_00;
    Item0887D2A8 items[10];
} Owner0887D2A8;

int func_0887CFDC(Owner0887D2A8 *o);
void func_0887D160(Owner0887D2A8 *o, int a, int b);

int func_0887D2A8(Owner0887D2A8 *o)
{
    if (func_0887CFDC(o) == 0) {
        return -1;
    }
    for (int i = 0; i < 10; i++) {
        Item0887D2A8 *it = &o->items[i];
        if (!it->active) {
            continue;
        }
        func_0887D160(o, it->a, it->b);
    }
    return 0;
}
}

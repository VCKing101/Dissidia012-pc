/* Resets every item of the object. */
struct Info08898EE8 {
    char pad_00[0x14];
    int count;
};

struct Obj08898EE8 {
    char pad_00[0x58];
    Info08898EE8 *info;
};

extern "C" {

void func_088981B4(Obj08898EE8 *o, int idx, int a, int b);

void func_08898EE8(Obj08898EE8 *o)
{
    if (o->info != 0) {
        for (int i = 0; i < o->info->count; i++) {
            func_088981B4(o, i, 1, 1);
        }
    }
}

}

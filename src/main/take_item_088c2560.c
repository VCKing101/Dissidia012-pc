/* Looks up an item by the id at +4 of src; if found, builds its record on the stack, stores it in the global list, plays its sound when flagged, and records the id. */
typedef struct Item { char p[2]; short snd; char q[4]; unsigned char flags; } Item;
typedef struct Sub { char p[0xF4]; char list[1]; } Sub;
typedef struct Ctx { char p[0x1FDD8]; Sub sub; } Ctx;
typedef struct Dst { int a; int id; } Dst;
typedef struct Src { int a; int id; } Src;
extern Ctx *D_089D712C;
extern Item *func_088BD278(int id);
extern void func_088F6208(void *buf, Item *it);
extern void func_088F526C(void *list, void *buf);
extern void func_088CC440(int snd);
int func_088C2560(int a0, Dst *dst, int a2, int a3, Src *src) {
    char buf[4264];
    Sub *sb = &D_089D712C->sub;
    int id = src->id;
    Item *it = func_088BD278(id);
    if (it != 0) {
        func_088F6208(buf, it);
        func_088F526C(sb->list, buf);
        if ((it->flags & 1) != 0) func_088CC440(it->snd);
        dst->id = id;
        return 1;
    }
    return 0;
}

/* Reloads the part set's owner model for the side, re-creates the parts and
 * attaches them. */
typedef struct PartSetView {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    unsigned char parts[0x128];
    int owner;
    void *source;
} PartSetView;

extern void func_089F7E40(void *parts);
extern void func_088DA060(void *model);
extern unsigned int func_08A22CA8(int owner);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_089F8100(void *parts, int kind, unsigned char flag);
extern int func_089F35C0(PartSetView *set);
extern int D_089F3830[2];

int func_089F370C(PartSetView *set, int kind, unsigned char flag)
{
    unsigned int side;

    if (!set->open) {
        return -1;
    }
    func_089F7E40(set->parts);
    if (set->model != 0) {
        func_088DA060(set->model);
        set->model = 0;
    }
    side = func_08A22CA8(set->owner);
    if (side >= 2) {
        return -2;
    }
    set->model = func_088DAA40(set->source, D_089F3830[side], 0);
    if (set->model == 0) {
        return -3;
    }
    func_089F8100(set->parts, kind, flag);
    func_089F35C0(set);
    return 0;
}

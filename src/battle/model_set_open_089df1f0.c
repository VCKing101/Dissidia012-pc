/* Opens the pool's model set: the base model and three part models looked up
 * from the source by name. */
typedef struct SetModel {
    unsigned char pad_00[0xE];
    short frame;
} SetModel;

typedef struct ModelSetView {
    unsigned char open;
    unsigned char pad_01[3];
    SetModel *base;
    SetModel *parts[3];
} ModelSetView;

typedef struct ModelName {
    int name;
    int frame;
} ModelName;

extern ModelName D_089DF4C8;
extern int D_089DF4F8[3];
extern void func_089DF170(ModelSetView *set);
extern SetModel *func_088DAA40(void *source, int name, int flags);
extern unsigned int func_089E9628(int index);

int func_089DF1F0(ModelSetView *set, void *source)
{
    func_089DF170(set);
    if (source == 0) {
        return -1;
    }
    set->base = func_088DAA40(source, D_089DF4C8.name, 0);
    if (set->base == 0) {
        return -2;
    }
    set->base->frame = D_089DF4C8.frame;
    {
        int i;
        for (i = 0; i < 3; i++) {
            unsigned int kind = func_089E9628(i);
            if (kind >= 3) {
                return -3;
            }
            set->parts[i] = func_088DAA40(source, D_089DF4F8[kind], 0);
            if (set->parts[i] == 0) {
                return -4;
            }
        }
    }
    set->open = 1;
    return 0;
}

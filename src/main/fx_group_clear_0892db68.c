/* Clears an effect group's motion, counters and blend values (-1 = none). */
typedef struct FxGroup {
    char motion[0x28];
    int f28;
    int f2C;
    int f30[2];
    int f38[1];
    int f3C;
    float blend;                /* 0x40 */
    float target;               /* 0x44 */
} FxGroup;

extern void func_0892AC2C(FxGroup *g);

void func_0892DB68(FxGroup *g)
{
    int i;
    func_0892AC2C(g);
    g->f28 = 0;
    g->f2C = 0;
    for (i = 0; i < 2; i++) {
        g->f30[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        g->f38[i] = 0;
    }
    g->f3C = 0;
    g->blend = 0.0f;
    g->target = -1.0f;
}

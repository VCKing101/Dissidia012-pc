typedef struct RankOwner {
    char pad_000[0x160];
    int id;
} RankOwner;

extern float func_08A9C0D8(RankOwner *self);
extern void *func_08AB327C(int id);
extern float func_08AB1ECC(void *unit);

float func_08A9C1C4(RankOwner *self)
{
    float base = func_08A9C0D8(self);
    return base * func_08AB1ECC(func_08AB327C(self->id));
}

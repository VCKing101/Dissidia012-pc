typedef struct Vec358510 {
    float x;
    float y;
    float z;
} Vec358510;

typedef struct Fighter58510 {
    char pad_000[0x3A8];
    Vec358510 *position;
} Fighter58510;

extern void func_08834C10(Fighter58510 *fighter, int state, int arg);

void func_08A58510(Fighter58510 *fighter, Vec358510 *position)
{
    Vec358510 *dst = fighter->position;
    func_08834C10(fighter, 0xB, 0);
    dst->x = position->x;
    dst->y = position->y;
    dst->z = position->z;
}

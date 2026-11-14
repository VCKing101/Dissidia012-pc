extern void func_0891191C(void *motion);

typedef struct MotionSet {
    char motion[0x14];
    int slots[5];               /* 0x14 */
} MotionSet;

void func_08911C5C(MotionSet *set)
{
    int i;
    func_0891191C(set->motion);
    for (i = 0; i < 5; i++) {
        set->slots[i] = 0;
    }
}

/* If the target is set: builds a matrix, scales it by the float argument and passes it to func_08835CB8 with the target. */
extern void func_089A3EBC(float *);
extern void func_089A3FF8(float *, float, float *);
extern void func_08835CB8(void *, float *);
void func_08B5A410(int unused, float f, void *target)
{
    float m[16] __attribute__((aligned(16)));
    if (target == 0) {
        return;
    }
    func_089A3EBC(m);
    func_089A3FF8(m, f, m);
    func_08835CB8(target, m);
}

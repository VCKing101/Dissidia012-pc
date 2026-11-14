/* Effect pools: each keeps a few running effects with their ages. Adding
   to a full pool releases the oldest effect and reuses its slot. */
struct PoolSlot08828638 {
    void *effect;
    float age;
};

extern "C" {

extern PoolSlot08828638 D_09AFB850[4];
extern PoolSlot08828638 D_09AFB870[2];
extern PoolSlot08828638 D_09AFB880[2];

void func_088A6C70(void *effect);

}

template <PoolSlot08828638 *pool, int count> struct Pool08828638 {
    static void add(void *effect)
    {
        for (int i = 0; i < count; i++) {
            PoolSlot08828638 *s = &pool[i];
            if (s->effect == 0) {
                s->effect = effect;
                return;
            }
        }
        int oldest = -1;
        for (int i = 0; i < count; i++) {
            if (oldest < 0) {
                oldest = i;
            } else if (pool[oldest].age < pool[i].age) {
                oldest = i;
            }
        }
        PoolSlot08828638 *s = &pool[oldest];
        func_088A6C70(s->effect);
        s->effect = effect;
        s->age = 0.0f;
    }
};

extern "C" {

void func_08828638(void *effect)
{
    Pool08828638<D_09AFB850, 4>::add(effect);
}

void func_08828704(void *effect)
{
    Pool08828638<D_09AFB870, 2>::add(effect);
}

void func_088287D0(void *effect)
{
    Pool08828638<D_09AFB880, 2>::add(effect);
}

}

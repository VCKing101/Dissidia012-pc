/* Releases every running effect in the three effect pools. */
struct PoolSlot0882889C {
    void *effect;
    float age;
};

extern "C" {

extern PoolSlot0882889C D_09AFB850[4];
extern PoolSlot0882889C D_09AFB870[2];
extern PoolSlot0882889C D_09AFB880[2];

void func_088A6C70(void *effect);

}

template <PoolSlot0882889C *pool, int count> struct Pool0882889C {
    static void clear()
    {
        for (int i = 0; i < count; i++) {
            PoolSlot0882889C *s = &pool[i];
            if (s->effect != 0) {
                func_088A6C70(s->effect);
                s->effect = 0;
                s->age = 0.0f;
            }
        }
    }
};

extern "C" {

void func_0882889C(void)
{
    Pool0882889C<D_09AFB850, 4>::clear();
    Pool0882889C<D_09AFB870, 2>::clear();
    Pool0882889C<D_09AFB880, 2>::clear();
}

}

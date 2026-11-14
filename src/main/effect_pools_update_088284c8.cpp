/* Ages the running effects of the three effect pools and frees the slots
   of effects that have ended. */
struct PoolSlot088284C8 {
    void *effect;
    float age;
};

extern "C" {

extern PoolSlot088284C8 D_09AFB850[4];
extern PoolSlot088284C8 D_09AFB870[2];
extern PoolSlot088284C8 D_09AFB880[2];
extern float D_089D5EAC;

int func_088A751C(void *effect);

}

template <PoolSlot088284C8 *pool, int count> struct Pool088284C8 {
    static void update()
    {
        float dt = D_089D5EAC;
        for (int i = 0; i < count; i++) {
            PoolSlot088284C8 *s = &pool[i];
            if (s->effect != 0) {
                if (func_088A751C(s->effect)) {
                    s->age += dt;
                } else {
                    s->effect = 0;
                    s->age = 0.0f;
                }
            }
        }
    }
};

extern "C" {

void func_088284C8(void)
{
    Pool088284C8<D_09AFB850, 4>::update();
    Pool088284C8<D_09AFB870, 2>::update();
    Pool088284C8<D_09AFB880, 2>::update();
}

}

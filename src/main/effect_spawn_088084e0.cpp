/* Spawns the hit effect at a position, if the effect system is up. */
struct Vec3_088084E0 {
    float x, y, z;
};

extern "C" {

extern int *D_089D6DD0;

void func_088088D0(void);
void *func_088A9E24(int system, void (*callback)(void), int id, int a, int b, int c);
void func_088A7284(void *effect, float x, float y, float z);
void func_08828704(void *effect);

}

struct Effect088084E0 {
    static Vec3_088084E0 copy(const Vec3_088084E0 *p) { return *p; }
    static void set_position(void *effect, const Vec3_088084E0 *pos)
    {
        Vec3_088084E0 v;
        v = copy(pos);
        func_088A7284(effect, v.x, v.y, v.z);
    }
};

extern "C" {

void func_088084E0(Vec3_088084E0 *pos, int id)
{
    int system = *D_089D6DD0;
    if (system != 0) {
        void *effect = func_088A9E24(system, func_088088D0, id, 0, 0, 0);
        if (effect != 0) {
            Effect088084E0::set_position(effect, pos);
            func_08828704(effect);
        }
    }
}

}

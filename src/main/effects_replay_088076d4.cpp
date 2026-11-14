
/* Restarts every recorded effect of the 32-entry effect table, then runs
   the table's follow-up pass. */
struct Point088076D4 {
    float x, y, z, w;

    Point088076D4(float px, float py, float pz) { x = px; y = py; z = pz; w = 0.0f; }
};

struct EffectRecord088076D4 {
    int owner;
    int param;
    float x;
    float y;
    float z;
    short id;
    signed char loop;
};

extern "C" {

extern EffectRecord088076D4 D_09AF84F0[32];

void func_08808AE8(int owner, int id, int param, Point088076D4 *pos, bool loop);
void func_0880766C(void);

void func_088076D4(void)
{
    for (int i = 0; i < 32; i++) {
        EffectRecord088076D4 *e = &D_09AF84F0[i];
        if (e->id >= 0) {
            Point088076D4 pos(e->x, e->y, e->z);
            func_08808AE8(e->owner, e->id, e->param, &pos, e->loop != 0);
        }
    }
    func_0880766C();
}

}

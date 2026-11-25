/* Looks up a vector with func_08925384(id), then calls func_088A7284(x, y + D_08B4BFE8, z, self->0xD0->0xC). */
struct Vec { float x, y, z; };
struct Inner { char pad[0xC]; int h; };
struct Self { char pad[0xD0]; struct Inner *in; };
extern float D_08B4BFE8;
extern struct Vec *func_08925384(int id);
extern void func_088A7284(float x, float y, float z, int h);
void func_08B3C3FC(struct Self *self, int id)
{
    struct Vec *v = func_08925384(id);
    float x = v->x;
    float y = v->y;
    float z = v->z;
    float zlift = D_08B4BFE8;
    float yy = y + zlift;
    func_088A7284(x, yy, z, self->in->h);
}

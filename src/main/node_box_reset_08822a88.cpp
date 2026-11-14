/* Starts a node's bounding box empty (min at +max, max at -max). */
struct NodeFlags08822A88 {
    unsigned int active : 1;
    unsigned int dirty : 1;
    unsigned int moved : 1;
    unsigned int box_min : 1;
    unsigned int box_max : 1;
};

struct Vec4_08822A88 {
    float x, y, z, w;

    static void set(Vec4_08822A88 *v, float s)
    {
        v->x = s;
        v->y = s;
        v->z = s;
        v->w = 1.0f;
    }
};

struct Node08822A88 {
    Vec4_08822A88 box_max;
    Vec4_08822A88 box_min;
    NodeFlags08822A88 flags;
};

extern "C" {

extern float D_089C76A0;

void func_089A3868(Vec4_08822A88 *dst, Vec4_08822A88 *src);

int func_08822A88(Node08822A88 *self)
{
    if (!self->flags.active) {
        return -1;
    }
    Vec4_08822A88 v;
    Vec4_08822A88::set(&v, -D_089C76A0);
    func_089A3868(&self->box_min, &v);
    Vec4_08822A88::set(&v, D_089C76A0);
    func_089A3868(&self->box_max, &v);
    self->flags.box_min = 1;
    self->flags.box_max = 1;
    return 0;
}

}

/* Copies a link's two end points from its owner: the bone point and the
   owner's own position. */
struct Vec4_08813010 {
    float x, y, z, w;
};

struct Owner08813010 {
    char data_00[0x70];
    virtual void v00();
    virtual void v01();
    virtual Vec4_08813010 bone_point(int bone);
    virtual Vec4_08813010 position();
};

struct Link08813010 {
    Vec4_08813010 from;
    Vec4_08813010 to;
    char pad_20[4];
    int bone;
    char pad_28[4];
    Owner08813010 *owner;
};

extern "C" {

void func_089A3868(Vec4_08813010 *dst, const Vec4_08813010 *src);

void func_08813010(Link08813010 *self)
{
    Vec4_08813010 v;
    v = self->owner->bone_point(self->bone);
    func_089A3868(&self->from, &v);
    v = self->owner->position();
    func_089A3868(&self->to, &v);
}

}

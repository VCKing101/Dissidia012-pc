/* Moves the owner's scene node to a point (w = 1) without interpolation.
   Retail's frame has 16 unused bytes below the vector (dead local). */
extern "C" {

typedef struct Vec3 {
    float x, y, z;
} Vec3;

struct Vec4 {
    float x, y, z, w;
    Vec4(const Vec3 &p, float pw) : x(p.x), y(p.y), z(p.z), w(pw) {}
};

typedef struct SceneModel {
    int field_00;
    void *node;                 /* 0x04 */
} SceneModel;

typedef struct NodeOwner {
    char pad_00[0xC];
    SceneModel *model;          /* 0x0C */
} NodeOwner;

extern void func_088359A8(void *node, Vec4 *pos, int flags);

void func_08921D74(NodeOwner *owner, Vec3 *pos)
{
    int unused[4];
    void *node = owner->model->node;
    Vec4 v(*pos, 1.0f);
    func_088359A8(node, &v, 0);
}

}

/* Moves the owner's scene node to a point (w = 1). Retail's frame has 16
   unused bytes below the vector: a dead local reproduces the layout. */
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

extern void func_08835BA4(void *node, Vec4 *pos);

void func_089216AC(NodeOwner *owner, Vec3 *pos)
{
    int unused[4];
    void *node = owner->model->node;
    Vec4 v(*pos, 1.0f);
    func_08835BA4(node, &v);
}

}

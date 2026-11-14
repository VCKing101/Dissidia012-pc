/* Returns the position of the owner's scene node by value. */
typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct SceneNode {
    char pad_00[0x80];
    Vec3 pos;                   /* 0x80 */
} SceneNode;

typedef struct SceneModel {
    int field_00;
    SceneNode *node;            /* 0x04 */
} SceneModel;

typedef struct NodeOwner {
    char pad_00[0xC];
    SceneModel *model;          /* 0x0C */
} NodeOwner;

Vec3 func_089216F4(NodeOwner *owner)
{
    Vec3 v = owner->model->node->pos;
    return v;
}

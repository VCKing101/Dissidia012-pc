/* Points the owner's model direction straight down (0, -1, 0). */
typedef struct SceneModel {
    char pad_00[0x14];
    float dir_x;                /* 0x14 */
    float dir_y;                /* 0x18 */
    float dir_z;                /* 0x1C */
} SceneModel;

typedef struct NodeOwner {
    char pad_00[0xC];
    SceneModel *model;          /* 0x0C */
} NodeOwner;

void func_08921DC0(NodeOwner *owner)
{
    owner->model->dir_x = 0.0f;
    owner->model->dir_y = -1.0f;
    owner->model->dir_z = 0.0f;
}

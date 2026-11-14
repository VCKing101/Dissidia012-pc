/* Blends the owner's node between two frames and sets mode 2. */
typedef struct SceneModel {
    int field_00;
    void *node;                 /* 0x04 */
} SceneModel;

typedef struct NodeOwner {
    unsigned int flags;         /* 0x00 */
    int field_04;
    int field_08;
    SceneModel *model;          /* 0x0C */
} NodeOwner;

extern void func_08B347AC(void *node, float from, float to);
extern void func_08921BA4(NodeOwner *owner, int mode, float value);

void func_08921B4C(NodeOwner *owner)
{
    float end = 60.0f;
    func_08B347AC(owner->model->node, 55.0f, end);
    func_08921BA4(owner, 2, end);
}

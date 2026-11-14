/* Starts the owner's node fade over sixty frames and marks it running. */
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

extern void func_08B347D4(void *node, float frames);
extern void func_08921BA4(NodeOwner *owner, int mode, float value);

void func_08921AC8(NodeOwner *owner)
{
    func_08B347D4(owner->model->node, 60.0f);
    func_08921BA4(owner, 0xE, 0.0f);
    owner->flags |= 8;
    owner->flags |= 2;
}

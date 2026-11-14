/* Resets a transform node: identity matrix, cleared vectors, no parent. */
typedef struct Vec4_0880FFE8 {
    float x;
    float y;
    float z;
    float w;
} Vec4_0880FFE8;

typedef struct Node0880FFE8 {
    float matrix[16];
    Vec4_0880FFE8 scale;
    Vec4_0880FFE8 rotation;
    Vec4_0880FFE8 position;
    char pad_70[0x24];
    int field_94;
    int field_98;
    char pad_9C[0x14];
    int field_B0;
    int field_B4;
    float field_B8;
    int field_BC;
    int parent;
} Node0880FFE8;

void func_089A3EBC(Node0880FFE8 *matrix);
void func_089A3890(Vec4_0880FFE8 *v);
void func_0880FEA0(Node0880FFE8 *node, int dirty, int flags);

void func_0880FFE8(Node0880FFE8 *node)
{
    node->field_94 = 0;
    node->field_98 = 0;
    node->field_B0 = 0;
    node->field_B4 = 0;
    node->field_B8 = 0.0f;
    func_089A3EBC(node);
    func_089A3890(&node->scale);
    node->scale.w = 1.0f;
    node->field_BC = 0;
    func_089A3890(&node->rotation);
    func_089A3890(&node->position);
    node->parent = -1;
    func_0880FEA0(node, 1, 0);
}

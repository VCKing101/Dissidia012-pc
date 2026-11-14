typedef struct Vec4 { float x; float y; float z; float w; } Vec4;
typedef struct Model { Vec4 rotation; int node; char data_14[0x28 - 0x14]; float scale; } Model;
typedef struct Scene { int field_00; int field_04; int root; } Scene;
extern Scene *D_089D6D80;
extern void func_089A3868(Vec4 *dst, Vec4 *src);
extern void func_08941A9C(int root, int node, Vec4 *v);

void func_088A46FC(Model *m, Vec4 *v)
{
    Vec4 scaled;
    int node;
    float w = v->w * m->scale;
    float x = v->x;
    float y = v->y;
    float z = v->z;
    scaled.x = x;
    scaled.y = y;
    scaled.z = z;
    scaled.w = w;
    func_089A3868(&m->rotation, v);
    node = m->node;
    func_08941A9C(D_089D6D80->root, node, &scaled);
}

typedef struct Vec3 { float x; float y; float z; } Vec3;
typedef struct Vec4 { float x; float y; float z; float w; } Vec4;
typedef struct Model { char data_00[0x10]; int node; } Model;
typedef struct Scene { int field_00; int field_04; int root; } Scene;
extern float D_089CAEF0;
extern Scene *D_089D6D80;
extern void func_08941A40(int root, int node, Vec4 *rotation);

void func_088A45D8(Model *m, Vec3 *radians)
{
    Vec4 degrees;
    float scale = 180.0f / D_089CAEF0;
    int node;
    degrees.x = scale * radians->x;
    degrees.y = scale * radians->y;
    degrees.z = scale * radians->z;
    degrees.w = 0.0f;
    node = m->node;
    func_08941A40(D_089D6D80->root, node, &degrees);
}

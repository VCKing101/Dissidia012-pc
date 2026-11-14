/* Rebuilds the node's world matrix from its local transform. */
typedef struct Mat0885EA14 {
    float m[12];
} Mat0885EA14;

typedef struct Vec4_0885EA14 {
    float x;
    float y;
    float z;
    float w;
} Vec4_0885EA14;

typedef struct Node0885EA14 {
    char pad_00[0x70];
    char local[0x40];
    char world[0x40];
} Node0885EA14;

extern char D_09B18E00[];

void func_089A3EFC(Mat0885EA14 *out, Node0885EA14 *node);
void func_089A383C(Vec4_0885EA14 *out, float x, float y, float z, float w);
void func_089A405C(void *dst, Mat0885EA14 *src);
void func_089A3F40(void *dst, void *a, void *b);

void func_0885EA14(Node0885EA14 *node)
{
    Mat0885EA14 m;
    Vec4_0885EA14 origin;
    func_089A3EFC(&m, node);
    func_089A383C(&origin, 0.0f, 0.0f, 0.0f, 1.0f);
    func_089A405C(node->local, &m);
    func_089A3F40(node->world, node->local, D_09B18E00);
}

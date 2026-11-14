/* Copies a node's transforms and pose into the render record. */
typedef struct Vec4_088A5F1C {
    float x;
    float y;
    float z;
    float w;
} Vec4_088A5F1C;

typedef struct Pose088A5F1C {
    Vec4_088A5F1C offset;
    char matrix[0x40];
} Pose088A5F1C;

typedef struct Node088A5F1C {
    char matrix[0x40];
    Vec4_088A5F1C scale;
    Vec4_088A5F1C rotation;
    Vec4_088A5F1C position;
    char world[0x40];
    Vec4_088A5F1C color;
} Node088A5F1C;

typedef struct Record088A5F1C {
    char pad_00[0x10];
    char matrix[0x40];
    Vec4_088A5F1C scale;
    Vec4_088A5F1C rotation;
    Vec4_088A5F1C position;
    char world[0x40];
    Vec4_088A5F1C color;
    Vec4_088A5F1C offset;
    char pose[0x40];
} Record088A5F1C;

void func_089A3EFC(void *dst, void *src);
void func_089A3868(Vec4_088A5F1C *dst, Vec4_088A5F1C *src);

void func_088A5F1C(Record088A5F1C *r, Node088A5F1C *n, Pose088A5F1C *p)
{
    func_089A3EFC(r->matrix, n->matrix);
    func_089A3868(&r->scale, &n->scale);
    func_089A3868(&r->rotation, &n->rotation);
    func_089A3868(&r->position, &n->position);
    func_089A3EFC(r->world, n->world);
    func_089A3868(&r->color, &n->color);
    r->offset.x = p->offset.x;
    r->offset.y = p->offset.y;
    r->offset.z = p->offset.z;
    r->offset.w = p->offset.w;
    func_089A3EFC(r->pose, p->matrix);
}

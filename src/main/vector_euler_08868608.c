typedef struct Agent2Vector {
    float x;
    float y;
    float z;
    float w;
} Agent2Vector;

extern void func_089A3868(Agent2Vector *, const Agent2Vector *);
extern float func_089A3BF8(const Agent2Vector *);
extern float func_089A457C(float, float);

void func_08868608(Agent2Vector *out, const Agent2Vector *direction) {
    Agent2Vector flat;
    Agent2Vector result;
    float length;
    func_089A3868(&flat, direction);
    flat.y = 0.0f;
    length = func_089A3BF8(&flat);
    result.x = func_089A457C(-direction->y, length);
    result.y = func_089A457C(direction->x, direction->z);
    result.z = 0.0f;
    result.w = 1.0f;
    *out = result;
}

float func_088686B0(const Agent2Vector *direction) {
    Agent2Vector flat;
    float length;
    func_089A3868(&flat, direction);
    flat.y = 0.0f;
    length = func_089A3BF8(&flat);
    return func_089A457C(-direction->y, length);
}

float func_088686FC(const Agent2Vector *direction) {
    return func_089A457C(direction->x, direction->z);
}

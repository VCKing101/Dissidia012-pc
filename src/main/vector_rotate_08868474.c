typedef struct Agent2Vector3 {
    float x;
    float y;
    float z;
} Agent2Vector3;

extern float func_089A46DC(float);
extern float func_089A47A4(float);

void func_08868474(Agent2Vector3 *out, const Agent2Vector3 *in, float angle) {
    float c = func_089A46DC(angle);
    float s = func_089A47A4(angle);
    float y = in->y;
    out->x = in->x;
    out->y = c * y + -(in->z * s);
    out->z = s * y + in->z * c;
}

void func_088684FC(Agent2Vector3 *out, const Agent2Vector3 *in, float angle) {
    float c = func_089A46DC(angle);
    float s = func_089A47A4(angle);
    float x = in->x;
    out->x = c * x + in->z * s;
    out->y = in->y;
    out->z = in->z * c - s * x;
}

void func_08868580(Agent2Vector3 *out, const Agent2Vector3 *in, float angle) {
    float c = func_089A46DC(angle);
    float s = func_089A47A4(angle);
    float x = in->x;
    out->x = c * x + -(in->y * s);
    out->y = s * x + in->y * c;
    out->z = in->z;
}

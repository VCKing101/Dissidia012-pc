typedef struct Agent2Vector {
    float x;
    float y;
    float z;
    float w;
} Agent2Vector;

extern float D_089C9468;
extern void func_089A3BC8(Agent2Vector *, const Agent2Vector *);
extern float func_089A3B10(const Agent2Vector *, const Agent2Vector *);
extern void func_089A3B30(Agent2Vector *, const Agent2Vector *,
                          const Agent2Vector *);
extern float func_089A4520(float);

float func_08867FEC(const Agent2Vector *first, const Agent2Vector *second) {
    Agent2Vector a;
    Agent2Vector b;
    float dot;
    func_089A3BC8(&a, first);
    func_089A3BC8(&b, second);
    dot = func_089A3B10(&a, &b);
    if (!(dot < 1.0f)) return 0.0f;
    if (dot <= -1.0f) return D_089C9468;
    return func_089A4520(dot);
}

float func_0886808C(const Agent2Vector *first, const Agent2Vector *second,
                    const Agent2Vector *up) {
    float angle = func_08867FEC(first, second);
    Agent2Vector cross;
    func_089A3B30(&cross, first, second);
    if (!(func_08867FEC(&cross, up) <= D_089C9468 / 180.0f * 90.0f)) {
        angle = -angle;
    }
    return angle;
}

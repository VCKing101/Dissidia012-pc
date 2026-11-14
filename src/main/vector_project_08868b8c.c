typedef struct Agent2Vector {
    float x;
    float y;
    float z;
    float w;
} Agent2Vector;

extern Agent2Vector func_0886871C(void *, void *, Agent2Vector *);
extern void func_089A3868(Agent2Vector *, const Agent2Vector *);
extern void func_089A3C18(Agent2Vector *, Agent2Vector *);

void func_08868B8C(void *first, void *second, Agent2Vector *out) {
    Agent2Vector value = func_0886871C(first, second, out);
    Agent2Vector copy;
    func_089A3868(&copy, &value);
    func_089A3C18(&copy, out);
}

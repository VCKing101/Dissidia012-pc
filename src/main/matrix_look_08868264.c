typedef struct Agent2Vector {
    float x;
    float y;
    float z;
    float w;
} Agent2Vector;

typedef struct Agent2Matrix {
    Agent2Vector row[4];
} Agent2Matrix;

extern void func_089A3868(Agent2Vector *, const Agent2Vector *);
extern void func_089A3B30(Agent2Vector *, const Agent2Vector *,
                          const Agent2Vector *);
extern void func_089A3BC8(Agent2Vector *, const Agent2Vector *);
extern void func_089A383C(Agent2Vector *, float, float, float, float);

void func_08868264(Agent2Matrix *matrix, const Agent2Vector *forward,
                   const Agent2Vector *up) {
    func_089A3868(&matrix->row[2], forward);
    func_089A3B30(&matrix->row[0], up, &matrix->row[2]);
    func_089A3BC8(&matrix->row[0], &matrix->row[0]);
    func_089A3B30(&matrix->row[1], &matrix->row[2], &matrix->row[0]);
    func_089A383C(&matrix->row[3], 0.0f, 0.0f, 0.0f, 1.0f);
}

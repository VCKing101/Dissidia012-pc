typedef struct Vec08870C94 {
    float x;
    float y;
    float z;
} Vec08870C94;

typedef struct Sphere08870C94 {
    Vec08870C94 center;
    float radius;
} Sphere08870C94;

void func_0885F644(Vec08870C94 *out, Sphere08870C94 *sphere, int target);
void func_089A3528(Vec08870C94 *out, Vec08870C94 *a, Sphere08870C94 *b);
float func_089A3718(Vec08870C94 *a, Vec08870C94 *b);

int func_08870C94(int target, Sphere08870C94 *sphere) {
    Vec08870C94 point;
    Vec08870C94 delta;
    func_0885F644(&point, sphere, target);
    func_089A3528(&delta, &point, sphere);
    if (func_089A3718(&delta, &delta) <= sphere->radius * sphere->radius) {
        return 1;
    }
    return 0;
}

// Builds a vector from func_08B3B60C(obj) via func_089A34C0, sets the object's matrix at +0x60 from it, advances the float at +0x64 by the table entry's +0x24 and copies the result to +0xC0.
extern void func_08B3B60C(void *, void *);
extern void func_089A34C0(float *, void *);
extern void func_089A383C(void *, float, float, float, float);
extern void func_089A3868(void *, void *);
typedef struct Entry57 {
    char pad0[0x24];
    float step;
    char pad1[0x94 - 0x28];
} Entry57;
extern Entry57 *D_089D4798;
typedef struct Obj57 {
    char pad0[0x60];
    char mat[4];
    float f64;
    char pad1[0xC0 - 0x68];
    char out[0x5C];
    int row;
} Obj57;
void func_08B3F500(int unused, Obj57 *obj)
{
    float a[4];
    float v[3];
    Entry57 *e = &D_089D4798[obj->row];
    func_08B3B60C(obj, a);
    func_089A34C0(v, a);
    func_089A383C(obj->mat, v[0], v[1], v[2], 1.0f);
    obj->f64 = obj->f64 + e->step;
    func_089A3868(obj->out, obj->mat);
}

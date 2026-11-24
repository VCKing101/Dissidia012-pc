/* Clears an object's first three words, a counter word and a float field. */
typedef struct { int a, b, c, d; float f; } Obj;
void func_08B436E8(Obj *o) {
    o->a = 0;
    o->b = 0;
    o->c = 0;
    o->f = 0.0f;
    o->d = 0;
}

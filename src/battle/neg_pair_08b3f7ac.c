/* Stores func_089A457C of the negated x and z of the target's position into the float at +8. */
extern float func_089A457C(float x, float z);
typedef struct { float x; float y; float z; } Vec;
typedef struct { char pad[0x90]; Vec v; } Pos;
typedef struct { char pad[0xF8]; Pos *pos; } Src;
typedef struct { char pad[8]; float r; } Dst;
void func_08B3F7AC(Dst *dst, Src *src) {
    Vec *p = &src->pos->v;
    dst->r = func_089A457C(-p->x, -p->z);
}

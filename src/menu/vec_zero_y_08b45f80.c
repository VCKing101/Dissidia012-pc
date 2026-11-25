/* Fetches a vector from a sub-object's link, zeroes its y and copies it out. */
typedef struct V3 { float x, y, z; } V3;
typedef struct Q { char pad[0x1C]; void *q; } Q;
typedef struct S { char pad[0x18]; Q *p; } S;
extern void func_089216F4(V3 *, void *);
void func_08B45F80(V3 *out, S *s) { V3 r; func_089216F4(&r, s->p->q); r.y = 0.0f; *out = r; }

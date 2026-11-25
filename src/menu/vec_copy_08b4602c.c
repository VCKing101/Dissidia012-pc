/* Calls a vector getter on the owner's sub-object at +0x20 and copies the result out. */
typedef struct V3 { float x, y, z; } V3;
typedef struct P { char pad[0x20]; V3 v; } P;
typedef struct S { char pad[0x18]; P *p; } S;
extern void func_08B4AE14(V3 *, V3 *);
void func_08B4602C(V3 *out, S *s) { V3 r; func_08B4AE14(&r, &s->p->v); *out = r; }

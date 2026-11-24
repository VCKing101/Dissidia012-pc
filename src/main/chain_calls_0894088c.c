/* Calls three functions in turn, the first with the field at +0x1C. */
typedef struct S { char pad[0x1C]; int h; } S;
extern void func_08949C94(int h, float a, float b);
extern void func_08970128(void);
extern void func_0896FB00(int x);
extern int D_09B6F470;
void func_0894088C(S *s, float a)
{
    func_08949C94(s->h, a, a);
    func_08970128();
    func_0896FB00(D_09B6F470);
}

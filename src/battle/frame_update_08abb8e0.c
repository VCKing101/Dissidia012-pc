/* Per-frame update: refreshes the three sub-objects and, unless already flagged, applies the frame step and marks the object; -1/0 early exits. */
typedef struct Obj38 { char pad[0xDA]; unsigned char done; unsigned char marked; } Obj38;
extern int func_08ABB474();
extern int func_08841F64(int p);
extern void func_08AB74E4(char *p);
extern void func_08ABA150(char *p);
extern void func_08AC0CF8(char *p);
extern void func_08ABBC48(float f, Obj38 *o);
extern void func_08AC2168(float f, char *p);
extern float D_089D5EAC;
int func_08ABB8E0(Obj38 *o)
{
    float f;
    if (func_08ABB474() == 0) {
        return -1;
    } else {
        if (func_08841F64(0) == 0) {
            return 0;
        } else {
            func_08AB74E4((char *)o + 0x1D8);
            func_08ABA150((char *)o + 0x1A4);
            func_08AC0CF8((char *)o + 0x1B8);
            if (o->done == 1) {
                return 0;
            } else {
                f = D_089D5EAC;
                func_08ABBC48(f, o);
                func_08AC2168(f, (char *)o + 0x16C);
                o->marked = 1;
                return 0;
            }
        }
    }
}

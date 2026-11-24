/* Passes the float at +0x140 of the idx-th 0xB0-byte entry and a second float on to func_08B3BE84. */
typedef struct { float f; char pad[0xAC]; } Ent;
typedef struct { char pad[0x140]; Ent e[1]; } Base;
extern void func_08B3BE84(float a, float b, Base *base, int idx);
void func_08B3BF68(float v, Base *base, int idx) {
    func_08B3BE84(base->e[idx].f, v, base, idx);
}

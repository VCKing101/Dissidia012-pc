/* Returns 1 when func_08AF9870(D_08B01CF8) is set or the global mode is 5 under D_089D5F20; else bit 0 of the flags word. */
typedef struct { char p0[0xC8]; int mode; } Glob;
typedef struct { char p0[0x1C]; unsigned int flags; } Obj;
extern int D_08B01CF8;
extern unsigned char D_089D5F20;
extern Glob D_09B6CDA0;
extern Obj *D_089D8140;
extern int func_08AF9870(int a);
unsigned int func_08AF96B0(void) {
    if (func_08AF9870(D_08B01CF8) != 0) {
        return 1;
    }
    if (D_089D5F20 != 0) {
        if (D_09B6CDA0.mode == 5) {
            return 1;
        } else {
        }
    }
    return D_089D8140->flags & 1;
}

/* If bit 0 of the flags word at D_089D8140+0x1C is clear, returns func_08AF4100(x); otherwise 0. */
typedef struct { char pad[0x1C]; unsigned int flags; } Obj;
extern Obj *D_089D8140;
extern int func_08AF4100(int x);
int func_08AF40C4(int x) {
    int a = 0;
    int b = D_089D8140->flags & 1;
    switch (b) {
    default:
        return 0;
    case 0:
        return func_08AF4100(x);
    }
}

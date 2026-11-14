/* Whether the menu root may go on: its lists are ready (func_08A1E1A8,
   func_08A20378) and the help window has closed. */
extern "C" {
extern void *D_08A23FF0;
extern void *D_089D69E8;
extern void func_08A1E168(void *root);
extern int func_08A1E1A8(void *root);
extern int func_08A20378(void *root);
extern int func_08885938(void *object);
extern void func_08885988(void *object);
extern int func_08885608(void *object, char value);
}

/* Inlined call on the object in D_089D69E8. */
struct Object9D69E8 {
    static int Accept(char value) { return func_08885608(D_089D69E8, value); }
};

extern "C" int func_08A0C294(void)
{
    func_08A1E168(D_08A23FF0);
    if (func_08A1E1A8(D_08A23FF0) == 0) {
        return 0;
    } else if (func_08A20378(D_08A23FF0) == 0) {
        return 0;
    } else if (func_08885938(D_089D69E8) != 0) {
        return 0;
    } else {
        func_08885988(D_089D69E8);
        if (Object9D69E8::Accept(1) == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}

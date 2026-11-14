/* Menu root: shows help text `text` in the help window (D_089D69E8), or
   just clears it for 0xFFFF. The calls are inlined wrappers with char
   parameters. */
extern "C" {
extern void *D_089D69E8;
void func_088852C4(void *object, char arg1, char arg2);
void func_088854C4(void *object, char kind, unsigned short text);
void func_08885890(void *object, char arg);
}

struct Object9D69E8 {
    static void Clear(char arg1, char arg2) { func_088852C4(D_089D69E8, arg1, arg2); }
    static void Show(char kind, unsigned short text) { func_088854C4(D_089D69E8, kind, text); }
    static void Open(char arg) { func_08885890(D_089D69E8, arg); }
};

extern "C" void func_08A1D4DC(void *self, unsigned short text)
{
    Object9D69E8::Clear(1, 0);
    if (text != 0xFFFF) {
        Object9D69E8::Show(9, text);
        Object9D69E8::Open(0);
    }
}

typedef struct CardScreen { char pad_00[0x28]; int mode; } CardScreen;
extern void func_0892C120(CardScreen *s);
extern void func_0892C148(CardScreen *s);
extern void func_0892C19C(CardScreen *s);
extern void func_0892C2A0(CardScreen *s);
void func_0892C0A8(CardScreen *s)
{
    switch (s->mode) {
    case 1:
        func_0892C120(s);
        func_0892C2A0(s);
        func_0892C148(s);
        break;
    case 2:
        func_0892C120(s);
        func_0892C148(s);
        func_0892C19C(s);
        break;
    }
}

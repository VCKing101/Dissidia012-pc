extern "C" {
extern void func_08935E30(short *dst, const short *src);
extern void func_0891E18C(void *label);
typedef struct Label {
    char pad_000[0x148];
    short text[0x20];           /* 0x148 */
    short icon;                 /* 0x188 */
    float scroll;               /* 0x18C */
    static void clear(Label *label) { label->text[0] = 0; label->icon = -1; label->scroll = 0.0f; }
} Label;
void func_0891E114(Label *label, const short *text)
{
    Label::clear(label);
    if (text != 0) {
        func_08935E30(label->text, text);
    }
    func_0891E18C(label);
}
}

extern "C" {
extern void func_0891E18C(void *label);
typedef struct Label {
    char pad_000[0x148];
    short text[0x20];           /* 0x148 */
    short icon;                 /* 0x188 */
    float scroll;               /* 0x18C */
    static void clear(Label *label) { label->text[0] = 0; label->scroll = 0.0f; }
} Label;
void func_0891E164(Label *label, int icon)
{
    Label::clear(label);
    label->icon = icon;
    func_0891E18C(label);
}
}

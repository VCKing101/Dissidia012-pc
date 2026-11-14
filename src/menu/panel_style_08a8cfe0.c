/* Resets panels 13 and 14 and draws panels 15 and 14 in style 12 (13 for mode 1). */
typedef struct StylePanels StylePanels;

extern void func_08A8DF30(StylePanels *self, int panel);
extern void func_08A8DB4C(StylePanels *self, int panel, int text, int arg);
extern void func_08A8D97C(StylePanels *self, int panel, int style, int a, int b);

void func_08A8CFE0(StylePanels *self, int text, int mode, int a, int b)
{
    int unused[1];
    int style[1];
    style[0] = 12;
    if (mode == 1) {
        style[0] = 13;
    }
    func_08A8DF30(self, 13);
    func_08A8DB4C(self, 13, text, -1);
    func_08A8DF30(self, 14);
    func_08A8D97C(self, 15, style[0], a, b);
    func_08A8D97C(self, 14, style[0], a, b);
}

/* Opens a two-part text box: the main text and its caption. */
struct TextBox088423A8 {
    char layout[0xC];
    char caption[0xC];
};

extern "C" {

extern char D_089C87B0[];
extern char D_089C87C8[];

void func_08842774(TextBox088423A8 *self, char *layout, int a, int b, char *style, int text);

void func_088423A8(TextBox088423A8 *self, int text, int a, int b, int c, int d)
{
    func_08842774(self, self->layout, a, b, D_089C87B0, text);
    func_08842774(self, self->caption, c, d, D_089C87C8, text);
}

}

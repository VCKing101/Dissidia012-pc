/* Opens a text box with the default style. */
struct TextBox08842420 {
    char pad_00[0x18];
    char layout[0xC];
    unsigned char mode;
};

extern "C" {

extern char D_089C87E0[];

void func_08842774(TextBox08842420 *self, char *layout, int a, int b, char *style, int text);

void func_08842420(TextBox08842420 *self, int text, int a, int b, unsigned char mode)
{
    self->mode = mode;
    func_08842774(self, self->layout, a, b, D_089C87E0, text);
}

}

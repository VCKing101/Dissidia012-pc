/* Formats the number into the label's text and lays out its two digit parts. */
struct NumberLabel {
    char field_00[0x4];
    char field_04[2][0x4];
};

extern "C" {
void func_089347E8(char *out, const char *format, int value);
void func_088D30E4(NumberLabel *label, char *part);
void func_088D321C(char *part, char *text);
void func_088D32F8(char *first, char *second);
void func_088D3374(char *part, int a, int b);
void func_088D3068(NumberLabel *label, char *part);
extern char D_08A768D0[];

void func_08A764B8(NumberLabel *self, int value)
{
    char text[0x10];
    func_089347E8(text, D_08A768D0, value);
    for (int i = 0; i < 2; i++) {
        func_088D30E4(self, self->field_04[i]);
    }
    func_088D321C(self->field_04[1], text);
    func_088D32F8(self->field_04[0], self->field_04[1]);
    for (int i = 0; i < 2; i++) {
        func_088D3374(self->field_04[i], 0, 5);
        func_088D3068(self, self->field_04[i]);
    }
}
}

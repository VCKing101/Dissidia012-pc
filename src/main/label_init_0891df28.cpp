extern "C" {
extern void func_08864FF0(void *view, int a, int b, int c);

typedef struct Label {
    char pad_000[0x100];
    char view[0x34];            /* 0x100 */
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    short text[0x20];           /* 0x148 */
    short icon;                 /* 0x188 */
    float scroll;               /* 0x18C */

    static Label *construct(Label *self)
    {
        func_08864FF0(self->view, 1, 0x11185, 0x50);
        self->field_134 = 0;
        self->field_138 = 0;
        self->field_13C = 0;
        self->field_140 = 0;
        self->field_144 = 0;
        self->text[0] = 0;
        self->icon = -1;
        self->scroll = 0.0f;
        return self;
    }
} Label;

Label *func_0891DF28(Label *self)
{
    return Label::construct(self);
}
}

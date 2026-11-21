#include "menu/agent36_types.h"

struct WorkD204 {
    char pad_0000[0xC];
    void *messages;           /* +0x000C */
    RootOptions36 *source;    /* +0x0010 */
    char pad_0014[0x6B1E];
    char text[0x202];         /* +0x6B32 */
    char pad_6D34[0x4];
    int mode;                 /* +0x6D38 */
    char pad_6D3C[0xA4];
    char *title;              /* +0x6DE0 */
    char pad_6DE4[0x10];
    int wait;                 /* +0x6DF4 */
    char pad_6DF8[0x4];
    int text_size;            /* +0x6DFC */
    char pad_6E00[0xC];
    char title_text[0x40];    /* +0x6E0C */
    char pad_6E4C[0x40];
    const char *template_text; /* +0x6E8C */
};

struct ScreenD204 {
    char pad_00[0x5C];
    WorkD204 *work;           /* +0x5C */
};

extern "C" {
extern void *D_08B59230;
void func_08929850(RootOptions36 *options, char *text, int size);
void func_089298A4(RootOptions36 *options, char *text, int size);
void func_08935524(void *dst, const void *src, int size);
const char *func_0887B500(void *messages, int group, int id);
void func_08B58DEC(void *p);
}

extern "C" void func_08A4D204(ScreenD204 *self)
{
    if (self->work->mode == 2) {
        func_08929850(self->work->source, self->work->text, sizeof(self->work->text));
        self->work->wait = 0xC;
    } else if (self->work->mode == 3) {
        func_089298A4(self->work->source, self->work->text, sizeof(self->work->text));
        self->work->wait = 0x78;
    } else if (self->work->mode == 4) {
        func_08935524(self->work->text, self->work->template_text, sizeof(self->work->text));
        self->work->wait = 0x32;
    }
    self->work->text_size = sizeof(self->work->text);

    int id = -1;
    switch (self->work->mode) {
    case 2:
        id = 0x1C;
        break;
    case 3:
        id = 0x1D;
        break;
    case 4:
        id = 0x1E;
        break;
    }
    func_08935524(self->work->title_text, func_0887B500(self->work->messages, 8, id), sizeof(self->work->title_text));
    self->work->title = self->work->title_text;
    func_08B58DEC(D_08B59230);
}

#include "menu/agent36_types.h"

/* Grid page: remakes texts 1 and 2 from the numbers at +0x1500/+0x1504
 * and plays their script. */
extern const char D_08A2E7D4[];
extern ScriptCmd36 D_08A2E61A[];

void func_08A2ABA8(GridScreen36 *self)
{
    void **texts = self->work->texts;
    char *lines[1];
    int one;

    if (texts[1] != 0) {
        func_0887BA5C(texts[1]);
        texts[1] = 0;
    }
    if (texts[2] != 0) {
        func_0887BA5C(texts[2]);
        texts[2] = 0;
    }
    one = 1;
    lines[0] = self->work->buffer;
    func_089347E8(self->work->buffer, D_08A2E7D4, self->work->field_1500);
    texts[1] = func_08A2BA30(self, 4, lines, one);
    func_089347E8(self->work->buffer, D_08A2E7D4, self->work->field_1504);
    texts[2] = func_08A2BA30(self, 3, lines, one);
    func_088D428C(self->work->handles, texts, D_08A2E61A, 1);
}

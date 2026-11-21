#include "menu/agent36_types.h"

typedef struct Work45ACC {
    char pad_000[0x8];
    void *messages;           /* +0x008 */
    void *texts[2];           /* +0x00C */
    char pad_014[0x90];
    char number[0x100];       /* +0x0A4 */
    char count[0x40];         /* +0x1A4 */
    char title[0x100];        /* +0x1E4 */
    char line[0x12C];         /* +0x2E4 */
    void *table;              /* +0x410 */
} Work45ACC;

typedef struct Screen45ACC {
    char pad_000[0x418];
    Work45ACC *work;          /* +0x418 */
} Screen45ACC;

extern char D_08A481E4[];
extern char D_08A481F0[];
int func_08A484D8(void *table, int column, int row);
void func_08A458A8(Screen45ACC *self, char *title, int size, const char *name, char *count);

void func_08A45ACC(Screen45ACC *self, int row)
{
    int values[2];
    char *lines[3];
    const char *name;

    values[0] = func_08A484D8(self->work->table, 2, row);
    values[1] = func_08A484D8(self->work->table, 6, row);
    lines[0] = self->work->number;
    lines[1] = self->work->title;
    lines[2] = self->work->line;
    func_089347E8(self->work->number, D_08A481E4, row + 1);
    name = func_0887B500(self->work->messages, 5, values[0]);
    func_089347E8(self->work->count, D_08A481F0, values[1]);
    func_08A458A8(self, self->work->title, 0x80, name, self->work->count);
    if (self->work->texts[0] != 0) {
        func_0887BA5C(self->work->texts[0]);
        self->work->texts[0] = 0;
    }
    if (self->work->texts[1] != 0) {
        func_0887BA5C(self->work->texts[1]);
        self->work->texts[1] = 0;
    }
    {
        int count = 2;
        self->work->texts[0] = func_08A412A8(D_08A457EC, 0x36, lines, count);
    }
    self->work->texts[1] = func_0887E534(self->work->texts[0]);
}

#include "menu/agent36_types.h"

/* Tabbed pages: shows seven lines of the current entry's long text from
 * the first line on (+0xCC); rows past its end lose their text. */
extern "C" void func_08A2F6F0(TabScreen36 *self, ListPage36 *page)
{
    int i;
    const char *line;
    TabRecord36 *record;

    record = TabList36::Record(page->list, TabList36::Id(self->work, page->list));
    line = func_0887B500(self->work->messages, page->kind, record->body);
    for (i = 0; i < self->work->lines.top; i++) {
        if (line != 0) {
            line = func_08A42E14(line);
        }
    }
    for (i = 0; i < 7; i++) {
        if (line != 0) {
            func_08A2F600(self, line, i);
            line = func_08A42E14(line);
        } else if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}

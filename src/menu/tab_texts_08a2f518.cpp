#include "menu/agent36_types.h"

/* Tabbed pages: makes the name and text of the current entry of the page's
 * list (texts 7 and 8) and plays their script. */

extern "C" ScriptCmd36 D_08A357CC[];

extern "C" void func_08A2F518(TabScreen36 *self, ListPage36 *page)
{
    TabRecord36 *record = TabList36::Record(page->list, TabList36::Id(self->work, page->list));

    self->work->texts[7] = func_0887B3EC(self->work->messages, page->kind, 0, record->name, 0, 0);
    self->work->texts[8] = func_0887B3EC(self->work->messages, page->kind, 0, record->text, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_08A357CC, 1);
}

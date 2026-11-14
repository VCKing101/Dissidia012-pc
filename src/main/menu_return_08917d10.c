extern void func_08865158(void *view);
extern void func_08916DAC(int page, int cursor);
extern void func_08916DFC(int page, int cursor);

typedef struct MenuPage {
    char pad_000[0x100];
    char view[0x34];            /* 0x100 */
    int selected;               /* 0x134 */
} MenuPage;

void func_08917D10(MenuPage *page)
{
    func_08865158(page->view);
    if (page->selected >= 0) {
        func_08916DFC(2, page->selected);
    } else {
        func_08916DAC(2, -1);
    }
}

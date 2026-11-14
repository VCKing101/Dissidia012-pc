typedef struct MenuState {
    int field_00;
    char view[0x34];            /* 0x04 */
    int cursor;                 /* 0x38 */
} MenuState;

extern void func_08916BB4(MenuState *menu, int page);
extern void func_088650A4(void *view);
extern void func_08917B80(int cursor);
extern MenuState *D_089D83D8;

void func_08916DFC(int page, int cursor)
{
    MenuState *menu = D_089D83D8;
    func_08916BB4(menu, page);
    func_088650A4(menu->view);
    if (cursor >= 0) {
        func_08917B80(cursor);
    }
}

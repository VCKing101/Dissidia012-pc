extern unsigned int D_089DE79C;
extern unsigned int D_089E330C;

/* Extra shutdown callback, selected by main 0x08933B4C through the table
 * entry at 0x089D85E4, following menu and battle shutdown callbacks. */
void func_088799DC(void) {
    D_089DE79C = 0;
    D_089E330C = 0;
}

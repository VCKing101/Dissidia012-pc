/* Starts the overlay-mode main task: func_08865088(D_089D85EC + 0x100, 0, 0, func_08933F50, "OVERLAY_MODE::MainTask"). */
extern char *D_089D85EC;
extern char D_089CF510[];
extern void func_08933F50(void);
extern void func_08865088(char *, int, int, void (*)(void), char *);
void func_08933C80(void) {
    func_08865088(D_089D85EC + 0x100, 0, 0, func_08933F50, D_089CF510);
}

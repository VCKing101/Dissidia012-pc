/* Per-event dispatch: on event 0x20 runs a step, then runs the handler for the event kind in each of the two phases when func_08AEDA5C reports it. */
extern void func_08AEDC74(void *a, int b, int c);
extern int func_08AEDA5C(void *a, int b, int phase);
extern void func_08AEDC80(void *a, int b, int c);
extern void func_08AEDF5C(void *a, int b, int c);
extern void func_08AEE82C(void *a, int b, int c);
extern void func_08AEDB98(void *a, int b, int c);
extern void func_08AEE038(void *a, int b, int c);
void func_08AEE6DC(void *a, int ev, int b, int c)
{
    if (ev == 0x20) {
        func_08AEDC74(a, b, c);
    }
    if (func_08AEDA5C(a, b, 1) == 1) {
        switch (ev) {
        case 2:
            func_08AEDC80(a, b, c);
            break;
        case 5:
            func_08AEDF5C(a, b, c);
        }
    }
    if (func_08AEDA5C(a, b, 0) == 1) {
        switch (ev) {
        case 0:
            func_08AEE82C(a, b, c);
            break;
        case 1:
            func_08AEDB98(a, b, c);
            break;
        case 0x17:
            func_08AEE038(a, b, c);
        }
    }
}

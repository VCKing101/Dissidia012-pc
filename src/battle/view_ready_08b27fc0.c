typedef struct ViewReady {
    unsigned char active;   /* 0x00 */
    char pad01[0x13];
    char request[4];        /* 0x14 */
} ViewReady;

extern int func_0887AFB0(void *request);

unsigned char func_08B27FC0(ViewReady *view)
{
    int result = 0;

    if (view->active == 1 && func_0887AFB0(view->request) == 1) {
        result = 1;
    }
    return result;
}

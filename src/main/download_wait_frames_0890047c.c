/* Frames to wait for the current download state: 0 when done, 150 when idle, else 60. */
extern int D_089D8130;

int func_0890047C(void)
{
    int state = D_089D8130;

    if (state == 2) {
        return 0;
    }
    if (state == 0) {
        return 150;
    }
    return 60;
}

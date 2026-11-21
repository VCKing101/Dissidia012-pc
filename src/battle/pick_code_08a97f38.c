/* Maps two small levels (0..2 and 0..2) queried from the object to a result code; -1 when out of range. */
extern int func_08A97934(int a, int b, int which);
int func_08A97F38(int a, int b)
{
    int x;
    int y;
    int r;
    if (b == 0) {
        return -1;
    } else {
        x = func_08A97934(a, b, 0);
        y = func_08A97934(a, b, 1);
        r = -1;
        switch (x) {
        case 0:
            switch (y) {
            case 0: r = 0x80; break;
            case 1:
            case 2: r = 0x7A; break;
            }
            break;
        case 1:
            switch (y) {
            case 0: r = 0x7B; break;
            case 1:
            case 2: r = 0x7D; break;
            }
            break;
        case 2:
            switch (y) {
            case 0: r = 0x7C; break;
            case 1:
            case 2: r = 0x7E; break;
            }
            break;
        }
        return r;
    }
}

/* Reads from the device (the count scaled to sectors unless the device is in byte mode); returns bytes read, 0 for end of media, -1 on error. */
typedef struct Dev { char p0[0x144]; unsigned int flags; char p1[4]; int handle; } Dev;
extern Dev *D_089D7F00;
extern int func_089C5E30(int handle, int a, int n);
int func_088EDAA4(int a, int n)
{
    int r;
    if (D_089D7F00->flags & 0x80) {
        r = func_089C5E30(D_089D7F00->handle, a, n);
    } else {
        r = func_089C5E30(D_089D7F00->handle, a, n * 2048);
    }
    if (r < 0) {
        if (r != 0x80210003) {
            return -1;
        } else {
            return 0;
        }
        if (r == 0x80210003) {
            return 0;
        }
        return -1;
    }
    if (r == 0) {
        return n;
    }
    if (D_089D7F00->flags & 0x80) {
        return r;
    }
    return r / 2048;
}

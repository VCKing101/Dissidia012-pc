/* Constructor: clear all fields of the object and store the owner argument. */
void func_08898DF8(int *p, int arg)
{
    int i;
    int *q;
    p[0x3C] = arg;
    p[0] = 0;
    p[0x23] = 0;
    p[0x2A] = 0;
    p[0x19] = 0;
    p[0x1D] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    i = 0;
    q = p;
    do {
        q[6] = 0;
        i++;
        q++;
    } while (i < 3);
    p[9] = 0;
    i = 0;
    q = p;
    do {
        q[10] = 0;
        i++;
        q++;
    } while (i < 3);
    p[0xD] = 0;
    ((char *)p)[0x38] = 0;
    p[0xF] = 0;
    p[0x10] = 0;
    p[0x11] = 0;
    p[0x12] = 0;
    p[0x13] = 0;
    p[0x14] = 0;
    p[0x15] = 0;
    p[0x16] = 0;
    p[0x17] = 0;
    p[0x18] = 0;
    p[0x1A] = 0;
    p[0x1B] = 0;
    p[0x1C] = 0;
    p[0x26] = 0;
    p[0x27] = 0;
    p[0x1E] = 0;
    ((char *)p)[0xC8] = 0;
    p[0x33] = 0;
    p[0x34] = 0;
    p[0x35] = 0;
    ((char *)p)[0xC0] = 0;
    p[0x2B] = 0;
    p[0x2C] = 0;
    p[0x2D] = 0;
    p[0x2E] = 0;
    p[0x2F] = 0;
    p[0x31] = 0;
    p[0x3D] = 0;
    p[0x3E] = 0;
    p[0x3F] = 0;
    p[0x40] = 0;
}

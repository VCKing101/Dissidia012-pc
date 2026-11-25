/* Scales the alpha byte of a packed colour by f (clamped to 255) and returns the repacked colour. */
unsigned int func_08925920(int unused, unsigned int c, float f) {
    unsigned int r = c & 0xFF;
    unsigned int g = (c >> 8) & 0xFF;
    unsigned int b = (c >> 16) & 0xFF;
    unsigned int a = (c >> 24) & 0xFF;
    unsigned int lo;
    unsigned int hi;
    a = (unsigned int)((float)a * f);
    if (a >= 0x100) a = 0xFF;
    g = g << 8;
    a = a << 24;
    b = b << 16;
    lo = g | r;
    hi = a | b;
    return lo | hi;
}

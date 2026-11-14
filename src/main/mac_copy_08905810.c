/* Copies a 6-byte MAC address and returns `dst`. */
typedef struct MacBytes {
    signed char bytes[6];
} MacBytes;

MacBytes *func_08905810(MacBytes *dst, const MacBytes *src)
{
    *dst = *src;
    return dst;
}

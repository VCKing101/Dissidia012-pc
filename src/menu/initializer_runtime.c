/* Descriptor-referenced initializer. The called main helper remains external;
 * this models the observed address/offset without asserting its full type. */
extern unsigned char D_09B6CDA0[];
extern void *func_088CB9E8(void *object);

void func_08AFFF54(void) {
    func_088CB9E8(D_09B6CDA0 + 0x244);
}

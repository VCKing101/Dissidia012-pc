/* Copies the data into the shared buffer and publishes it, unless one is already published. */
typedef struct Buffer088B70EC {
    char data[1];
} Buffer088B70EC;

extern int *D_089D7368;
extern Buffer088B70EC D_09B4D4A0;
void *func_08935524(void *dst, const void *src, int size);
void func_088B706C(int *out, Buffer088B70EC *value);
void func_088B70EC(const void *src, int size)
{
    if (*D_089D7368 != 0) {
        return;
    }
    func_08935524(&D_09B4D4A0, src, size);
    func_088B706C(D_089D7368, &D_09B4D4A0);
}

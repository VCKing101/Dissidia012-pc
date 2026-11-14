/* Copies the data into the shared buffer and publishes it, unless one is already published. */
typedef struct Buffer088C1B9C {
    char data[1];
} Buffer088C1B9C;

extern int *D_089D7438;
extern Buffer088C1B9C D_09B692B0;
void *func_08935524(void *dst, const void *src, int size);
void func_088C1B48(int *out, Buffer088C1B9C *value);
void func_088C1B9C(const void *src, int size)
{
    if (*D_089D7438 != 0) {
        return;
    }
    func_08935524(&D_09B692B0, src, size);
    func_088C1B48(D_089D7438, &D_09B692B0);
}

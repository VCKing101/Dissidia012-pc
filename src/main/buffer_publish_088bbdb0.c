/* Copies the data into the shared buffer and publishes it, unless one is already published. */
typedef struct Buffer088BBDB0 {
    char data[1];
} Buffer088BBDB0;

extern int *D_089D73A8;
extern Buffer088BBDB0 D_09B4E858;
void *func_08935524(void *dst, const void *src, int size);
void func_088BBD44(int *out, Buffer088BBDB0 *value);
void func_088BBDB0(const void *src, int size)
{
    if (*D_089D73A8 != 0) {
        return;
    }
    func_08935524(&D_09B4E858, src, size);
    func_088BBD44(D_089D73A8, &D_09B4E858);
}

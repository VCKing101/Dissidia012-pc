/* Copies the data into the shared buffer and publishes it, unless one is already published. */
typedef struct Buffer088C0A44 {
    char data[1];
} Buffer088C0A44;

extern int *D_089D73FC;
extern Buffer088C0A44 D_09B68B58;
void *func_08935524(void *dst, const void *src, int size);
void func_088C0964(int *out, Buffer088C0A44 *value);
void func_088C0A44(const void *src, int size)
{
    if (*D_089D73FC != 0) {
        return;
    }
    func_08935524(&D_09B68B58, src, size);
    func_088C0964(D_089D73FC, &D_09B68B58);
}

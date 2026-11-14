extern void func_088DA0A8(void *src, int bone, float *matrix, char *flags);
extern void func_088D9B84(void *dst, float *matrix);
extern void func_088D9AF4(void *dst, char *flags);
void func_0892B7F8(void *self, void *src, void *dst, int bone)
{
    float matrix[16];
    char flags[4];
    if (src == 0) {
        return;
    }
    if (dst == 0) {
        return;
    }
    func_088DA0A8(src, bone, matrix, flags);
    func_088D9B84(dst, matrix);
    func_088D9AF4(dst, flags);
}

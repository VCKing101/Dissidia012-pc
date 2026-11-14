extern void func_08AB40FC(void *self, float x, float y, float z);

/* Same scale on all three axes. */
void func_08AB40DC(void *self, float scale)
{
    func_08AB40FC(self, scale, scale, scale);
}

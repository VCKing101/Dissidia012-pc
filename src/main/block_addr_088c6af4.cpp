/* Address of block n: the sum of the sizes of the blocks before it, as words into the loaded data. */
struct SizeInfo088C6AF4 { char pad[4]; unsigned char sizes[1]; };
extern "C" {
extern SizeInfo088C6AF4 *D_089D7728;
extern int *D_089D772C;
int *func_088C6AF4(int n)
{
    if (D_089D772C != 0) {
        int offset = 0;
        for (int i = 0; i < n; i++) {
            offset += D_089D7728->sizes[i];
        }
        return &D_089D772C[offset];
    }
    return 0;
}
}

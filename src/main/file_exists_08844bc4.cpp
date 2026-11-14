/* Whether the data file of an entry exists. */
struct Info08844BC4 {
    char pad_00[9];
    unsigned char packed;
    char pad_0A[0x24 - 0xA];
    char name[1];
};

extern "C" {

extern char D_089C88A8[];

Info08844BC4 *func_088470B8(int id);
char *func_08844FA4(char *name, char *format);
int func_088FE7C8(char *path, bool packed);

bool func_08844BC4(int id)
{
    Info08844BC4 *info = func_088470B8(id);
    char *path = func_08844FA4(info->name, D_089C88A8);
    return 0 < func_088FE7C8(path, 0 < info->packed);
}

}

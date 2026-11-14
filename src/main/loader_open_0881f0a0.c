typedef struct Loader {
    unsigned char loaded;
    char pad1[0x3];
    void *data;
    char textures[0x1C];
    char models[0x10];
} Loader;
extern void func_0881F068(Loader *loader);
extern int func_0881F4D8(void *textures, void *data);
extern int func_0882028C(void *models, void *data);
int func_0881F0A0(Loader *loader, void *data)
{
    func_0881F068(loader);
    if (data == 0) {
        return -1;
    }
    if (func_0881F4D8(loader->textures, data) != 0) {
        return -2;
    }
    if (func_0882028C(loader->models, data) != 0) {
        return -3;
    }
    loader->data = data;
    loader->loaded = 1;
    return 0;
}

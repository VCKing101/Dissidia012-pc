/* Resets the light; unless `keep`, releases the handle at 8 and clears 8/0xC. */
typedef struct LightHandle {
    char pad_00[8];
    void *handle;
    int field_0C;
} LightHandle;

extern void func_088E67DC(LightHandle *light);
extern void func_088665C4(void *handle);

void func_088E6794(LightHandle *light, unsigned char keep)
{
    func_088E67DC(light);
    if (!keep) {
        func_088665C4(light->handle);
        light->handle = 0;
        light->field_0C = 0;
    }
}

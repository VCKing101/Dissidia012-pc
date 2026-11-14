extern void func_088D6DBC(void *p);
extern void func_088DE378(void *p);
extern void func_088CD4C8(void *p);
extern void func_0892F5AC(void *p);
extern void func_088CE3D8(void *p);

typedef struct StageData {
    char pad_000[0xA0];
    char models[0x31C - 0xA0];      /* 0x0A0 */
    char effects[0x3C0 - 0x31C];    /* 0x31C */
    char sounds[0x404 - 0x3C0];     /* 0x3C0 */
    char lights[0x418 - 0x404];     /* 0x404 */
    char camera[0x7D7 - 0x418];     /* 0x418 */
    unsigned char loaded;           /* 0x7D7 */
} StageData;

void func_08913810(StageData *stage)
{
    if (stage->loaded == 0) {
        return;
    }
    func_088D6DBC(stage->models);
    func_088DE378(stage->effects);
    func_088CD4C8(stage->sounds);
    func_0892F5AC(stage->camera);
    func_088CE3D8(stage->lights);
    stage->loaded = 0;
}

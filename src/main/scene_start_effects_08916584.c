#include "main/agent21_types.h"

extern int func_088729A0(int id);
extern void func_088DE6D4(void *effects, int bank, int arg, int on);
extern void func_088D7574(void *models);
extern void func_088CDB04(void *sounds);

typedef struct SceneParts {
    char pad_000[0xA0];
    char models[0x31C - 0xA0];      /* 0x0A0 */
    char effects[0x3C0 - 0x31C];    /* 0x31C */
    char sounds[0x7E3 - 0x3C0];     /* 0x3C0 */
    unsigned char started;          /* 0x7E3 */
} SceneParts;

void func_08916584(SceneParts *scene, int arg)
{
    scene->started = 1;
    func_088DE6D4(scene->effects, func_088729A0(0), arg, 1);
    func_088D7574(scene->models);
    func_088CDB04(scene->sounds);
}

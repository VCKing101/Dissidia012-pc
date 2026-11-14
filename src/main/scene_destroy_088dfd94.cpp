/* Scene deleting destructor: member destructors in reverse order, then the delete. */
extern "C" {
extern char D_089CC358[];
extern void func_088EA2DC(void *member, int flags);
extern void func_088EAA24(void *member, int flags);
extern void func_088E66B4(void *member, int flags);
extern void func_088EA760(void *member, int flags);
extern void func_08869908(void *member, int flags);
extern void func_088E81FC(void *member, int flags);
extern void func_088E6E18(void *member, int flags);
extern void func_088E5B04(void *member, int flags);
extern void func_088E2AC8(void *member, int flags);
extern void func_08992F04(void *p);
}

struct SceneList {
    char data[0x1C];

    static void destroy(SceneList *list)
    {
        if (list != 0) {
            func_08869908(list, 2);
        }
    }
};

struct Scene {
    char pad_000[0x5BC];
    char view[0xD7C - 0x5BC];
    char stage[0xE00 - 0xD7C];
    char effects[0x1220 - 0xE00];
    char actors[0x1270 - 0x1220];
    SceneList list;
    char camera[0x12B0 - 0x128C];
    char lights[0x12D8 - 0x12B0];
    char sounds[0x135C - 0x12D8];
    char hud[0x17C4 - 0x135C];
    void *vtable;

    static void destroy(Scene *scene, unsigned int flags, void *vt)
    {
        if (scene != 0) {
            scene->vtable = vt;
            func_088EA2DC(scene->hud, 2);
            func_088EAA24(scene->sounds, 2);
            func_088E66B4(scene->lights, 2);
            func_088EA760(scene->camera, 2);
            SceneList::destroy(&scene->list);
            func_088E81FC(scene->actors, 2);
            func_088E6E18(scene->effects, 2);
            func_088E5B04(scene->stage, 2);
            func_088E2AC8(scene->view, 2);
            if (flags & 1) {
                func_08992F04(scene);
            }
        }
    }
};

extern "C" {
void func_088DFD94(Scene *scene, unsigned int flags)
{
    Scene::destroy(scene, flags, D_089CC358);
}
}

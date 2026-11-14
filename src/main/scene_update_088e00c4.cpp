/* Scene per-frame update: view, stage, actors, camera, lights, sounds, then globals. */
extern "C" {
extern char *D_089D5EC0;
extern void *D_089D6040;
extern void *D_089D5E88;
extern const float D_089C9B40;
extern const float D_089C9B44;
extern void func_088E2DD4(void *view);
extern void func_088E5B58(void *stage);
extern void func_088E0814(void *scene);
extern void func_088EA868(void *camera);
extern void func_088E6764(void *lights);
extern void func_088EAC08(void *sounds);
extern void func_088EA368(void *hud);
extern void func_08860EB0(void);
extern void func_08873594(void *target, float x, float y);
extern void func_088759C0(void *p);
extern void func_08861098(void *p);
extern void func_088664FC(void *handle);
}

struct SceneOwner {
    char pad_0[6];
    unsigned short flags;

    static bool hasFlag4(SceneOwner *owner)
    {
        return (owner->flags & 4) != 0;
    }
};

struct SceneU {
    char pad_000[8];
    SceneOwner *owner;
    char pad_00C[0x5BC - 0xC];
    char view[0xD7C - 0x5BC];
    char stage[0x128C - 0xD7C];
    char camera[0x12B0 - 0x128C];
    char lights[0x12D8 - 0x12B0];
    char sounds[0x135C - 0x12D8];
    char hud[0x162C - 0x135C];
    void *handle;
    int handle_info;
    char pad_1634[0x17B9 - 0x1634];
    unsigned char paused;
    unsigned char lights_on;
};

extern "C" {
void func_088E00C4(SceneU *scene)
{
    func_088E2DD4(scene->view);
    func_088E5B58(scene->stage);
    if (scene->paused == 0) {
        func_088E0814(scene);
    }
    if (scene->paused == 0) {
        func_088EA868(scene->camera);
    }
    if (scene->lights_on != 0) {
        func_088E6764(scene->lights);
    }
    if (scene->paused == 0) {
        func_088EAC08(scene->sounds);
    }
    func_088EA368(scene->hud);
    if (scene->paused == 0) {
        func_08860EB0();
    }
    func_08873594(D_089D5EC0 + 0x7E0, D_089C9B40, D_089C9B44);
    func_088759C0(D_089D6040);
    func_08861098(D_089D5E88);
    if (SceneOwner::hasFlag4(scene->owner)) {
        scene->handle_info = 0;
        func_088664FC(scene->handle);
        scene->handle = 0;
    }
}
}

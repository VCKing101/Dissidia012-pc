typedef struct Stage {
    char pad0[0x570];
    char lights[0x40];
    char models[0x2C];
    char sounds[0x18];
    void *scene;
    void *archive;
} Stage;
extern void func_08842138(void *object);
extern void func_0881F068(void *scene);
extern void func_0882117C(void *sounds);
extern void func_08817D18(void *models);
extern void func_088223A8(void *lights);
extern void func_08814C94(Stage *stage);
void func_08814D08(Stage *stage)
{
    if (stage->archive != 0) {
        func_08842138(stage->archive);
    }
    if (stage->scene != 0) {
        func_0881F068(stage->scene);
        func_08842138(stage->scene);
    }
    func_0882117C(stage->sounds);
    func_08817D18(stage->models);
    func_088223A8(stage->lights);
    func_08814C94(stage);
}

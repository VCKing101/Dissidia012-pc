extern "C" {
struct Part {
    char pad0[0x44];
};
struct Model {
    short loaded;
    short count;
    Part *parts;
    static unsigned char isLoaded(Model *model) { return model->loaded != 0; }
};
extern void func_0881FB68(Part *part, unsigned char visible);
int func_0881F79C(Model *model, unsigned char visible)
{
    if (!Model::isLoaded(model)) {
        return -1;
    }
    for (int i = 0; i < model->count; i++) {
        func_0881FB68(&model->parts[i], visible);
    }
    return 0;
}
}

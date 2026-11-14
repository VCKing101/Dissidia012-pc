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
extern void func_0881FA30(Part *part);
int func_0881F688(Model *model)
{
    if (!Model::isLoaded(model)) {
        return -1;
    }
    for (int i = 0; i < model->count; i++) {
        func_0881FA30(&model->parts[i]);
    }
    return 0;
}
}

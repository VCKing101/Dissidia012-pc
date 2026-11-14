typedef struct Model { char data_00[0xE]; short pose; } Model;
typedef struct Scene { char data_00[0x24]; Model *model; char data_28[4]; int state; unsigned char done; } Scene;
extern void func_088D9F5C(Model *m, int pose, unsigned char now);

void func_088ECD18(Scene *s)
{
    switch (s->state) {
    case 0:
        func_088D9F5C(s->model, 0, 1);
        s->model->pose = 1;
        break;
    case 3:
        s->done = 1;
        break;
    }
}

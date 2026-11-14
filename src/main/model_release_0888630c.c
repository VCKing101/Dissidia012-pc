typedef struct Model { char data_00[0xC]; short state; } Model;
typedef struct Holder { void *owner; Model *model; } Holder;
extern void func_088D9F5C(Model *model, int pose, int now);
extern void func_088DB10C(void *owner);
extern void func_088DA060(Model *model);

void func_0888630C(Holder *h)
{
    if (h->owner != 0 && h->model != 0) {
        if (h->model->state == 1) {
            func_088D9F5C(h->model, 2, 0);
        } else {
            func_088DB10C(h->owner);
            if (h->model != 0) {
                func_088DA060(h->model);
                h->model = 0;
            }
        }
    }
}

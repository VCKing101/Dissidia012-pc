/* Starts a camera move on the current scene target's view point (-1
 * closed, -2 no target, -3 target not ready, -4 no model, -5 no view, -6
 * the move was refused). */
template <class T> struct SceneKept {
    T value;
    SceneKept(T v) : value(v) {}
};

struct SceneView {
    unsigned char pad_00[0x3C];
    unsigned char point[4];
};

struct SceneModel {
    unsigned char pad_00[0x24];
    SceneView *view;
};

struct SceneTarget {
    unsigned char pad_00[0x64];
    SceneModel *model;
};

struct SceneEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
};

extern float D_08A7ABE4;

extern "C" {

extern void *func_08AB3274(void *owner);
extern SceneTarget *func_08A827AC(void *settings);
extern int func_08AD77E8(SceneTarget *target);
extern int func_08A72B68(SceneEvent *self, void *point, int mode, unsigned char a, float time, unsigned char b,
                         unsigned char c, int d);

int func_08A6656C(SceneEvent *self, unsigned char a, unsigned char b, unsigned char c, int d)
{
    if (!self->open) {
        return -1;
    }
    SceneTarget *target = func_08A827AC(func_08AB3274(self->owner));
    if (target == 0) {
        return -2;
    }
    SceneKept<SceneTarget *> saved(target);
    int ready = func_08AD77E8(target);
    target = saved.value;
    if (ready == 0) {
        return -3;
    }
    SceneModel *model = target->model;
    if (model == 0) {
        return -4;
    }
    SceneView *view = model->view;
    if (view == 0) {
        return -5;
    }
    if (func_08A72B68(self, view->point, 4, a, D_08A7ABE4, b, c, d) != 0) {
        return -6;
    }
    return 0;
}

}

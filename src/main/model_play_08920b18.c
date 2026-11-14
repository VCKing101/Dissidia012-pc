/* Starts a motion on the owner's model unless the model is busy. */
typedef struct ModelRef {
    int handle;                 /* 0x00 */
} ModelRef;

typedef struct ModelOwner {
    char pad_00[0xC];
    ModelRef *model;            /* 0x0C */
} ModelOwner;

extern int D_089D4960;
extern int *func_088444D4(int manager, int handle);
extern void func_08844C14(int handle, int motion, int flags);

void func_08920B18(ModelOwner *owner, int motion)
{
    int handle = owner->model->handle;
    if (func_088444D4(D_089D4960, handle) == 0) {
        func_08844C14(owner->model->handle, motion, 0);
    }
}

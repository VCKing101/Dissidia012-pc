/* Stops the owner's model motion when it is playing. The lookup sits in an
   in-class helper: that loads the global manager into a temporary first. */
extern "C" {

typedef struct ModelRef {
    int handle;                 /* 0x00 */
} ModelRef;

typedef struct ModelOwner {
    char pad_00[0xC];
    ModelRef *model;            /* 0x0C */
} ModelOwner;

extern int D_089D4960;
extern int *func_088444D4(int manager, int handle);
extern void func_08844DE0(int handle);

struct ModelOps {
    static int *find(int handle) { return func_088444D4(D_089D4960, handle); }
};

void func_08920B74(ModelOwner *owner)
{
    if (ModelOps::find(owner->model->handle) != 0) {
        func_08844DE0(owner->model->handle);
    }
}

}

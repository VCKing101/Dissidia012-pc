#include "menu/agent29_types.h"

/* Screen0B6A0: plays the record board's script, then for each of the five
   record places hides the model when the place is empty, or plays the
   done motion (when marked done) and loads the record's picture
   (0x3427 + id) into its layout entry. */
extern "C" {
extern const unsigned short D_08A0A9CA[];
extern void *D_089D7830;
void func_088D0984(void *loader, void *entry, int kind, unsigned short picture);
}

/* Inlined calls on the loader in D_089D7830. */
struct Loader9D7830 {
    static void Picture(void *entry, char kind, unsigned short picture) {
        func_088D0984(D_089D7830, entry, kind, picture);
    }
};

extern "C" void func_089FC91C(Screen0B6A0 *self)
{
    int i;
    int id;
    unsigned int state;

    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A9CA);
    for (i = 0; i < 5; i++) {
        id = self->work->field_2E84[i];
        state = (&WORK_INT(self->work, 0x2E98))[i];
        if (id == 0) {
            func_088D9CFC(self->work->handles[5 + i]);
        } else {
            if (state & 1) {
                func_088D9F5C(self->work->handles[5 + i], 0x1E, 1);
            }
            Loader9D7830::Picture(self->work->layout.entries[1 + i], 0xE, id + 0x3427);
        }
    }
    func_089FC9FC(self);
}

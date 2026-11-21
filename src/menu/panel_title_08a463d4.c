#include "menu/agent36_types.h"

/* Panel: remakes texts 2 and 3 (message id of kind 5) unless keep is set,
 * and plays their script. */

typedef struct Work463D4 {
    int field_00;
    void *handles;            /* +0x04 */
    char pad_08[0x4];
    void *texts[4];           /* +0x0C */
} Work463D4;

typedef struct Screen463D4 {
    char pad_000[0x418];
    Work463D4 *work;          /* +0x418 */
} Screen463D4;

extern ScriptCmd36 D_08A48070[];

void func_08A463D4(Screen463D4 *self, int keep, int id)
{
    void *messages = D_08A457EC->table->messages;

    if (keep == 0) {
        if (self->work->texts[2] != 0) {
            func_0887BA5C(self->work->texts[2]);
            self->work->texts[2] = 0;
        }
        if (self->work->texts[3] != 0) {
            func_0887BA5C(self->work->texts[3]);
            self->work->texts[3] = 0;
        }
        self->work->texts[2] = func_0887B3EC(messages, 5, 0, id, 0, 0);
        self->work->texts[3] = func_0887E534(self->work->texts[2]);
    }
    func_088D428C(self->work->handles, self->work->texts, D_08A48070, 1);
}

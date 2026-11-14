#include "menu/auto_region3.h"

/* View8E3B4: enters mode 0..3. */
void func_08A932D0(View8E3B4 *self, int mode)
{
    Work8E3B4 *work;
    int record;

    if (mode == 0) {
        self->work->field_2AC = 0x19A;
        self->work->field_2C4 = 0;
        func_08A924A8(self, 1, 2, self->work->field_2C4);
        func_08A8DF30(self, 5);
        func_08A8DB4C(self, 5, -1, -1);
        func_0887B924(self->work->texts[5]);
        func_088D9CD8(self->work->handles[8]);
        func_08A8D70C(self, 0, 0, 1);
        func_088D9CD8(self->work->handles[9]);
        func_0887B924(self->work->texts[0]);
        func_08A8CCE0(self, 1, 1);
        func_08A8A440(D_08A8B7A8, 0, 0);
        self->work->field_2CC = 4;
        func_08A8C540(self);
        func_08A8EF2C(self, 1, 2);
    }
    else if (mode == 1) {
        self->work->field_2AC = 100;
        func_0887B944(self->work->texts[0]);
        func_08A8C540(self);
        func_08A8EF2C(self, 0, 0);
    }
    else if (mode == 2) {
        work = self->work;
        record = work->field_308[work->index];
        work->field_2AC = 0x136;
        self->work->field_2C4 = 0;
        func_08A924A8(self, 1, 2, self->work->field_2C4);
        func_08A8DA6C(self, -1);
        func_0887B924(self->work->texts[4]);
        func_08A8D27C(self, 1);
        func_08A8CCE0(self, 1, 1);
        func_08A8C46C(self, 1);
        func_08A94C04(&self->field_0C, 1, &self->work->records[record], self->work->handles);
        func_08A8A440(D_08A8B7A8, 0, 0);
        func_08A8C540(self);
        func_08A8EF2C(self, 1, 2);
    }
    else if (mode == 3) {
        work = self->work;
        record = work->field_308[work->index];
        work->field_2AC = 200;
        func_08A91FD4(self, 1, -1, 0);
        func_08A8CD90(self, 1, -1);
        func_08A94A1C(&self->field_0C, &self->work->records[record]);
        work = self->work;
        func_08A95A38(&self->field_0C, &work->records[record], work->layout, work->handles, 1);
        func_08A8A440(D_08A8B7A8, 1, 0);
        func_08A8C540(self);
        func_08A8EF2C(self, 0, 0);
    }
    func_08A8EF90(self, 10);
}

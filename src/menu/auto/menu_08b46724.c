#include "menu/auto_region4.h"

void func_08B46724(Net45BB8 *self, int arg)
{
    int count;
    int id;
    int ids[70];
    void *object;

    object = self->data->field_1C;
    func_08B46698(self, 1);
    id = func_089212E8(object);
    count = func_08B47880(self, id, ids, 1);
    if (0 < count) {
        self->data->field_148 = ids[0];
        id = func_08B477E8(self, self->data->field_148, self->data->field_2C, arg);
        self->data->field_144 = id;
    }
}

/* True when the indexed entry passes the slot checks and its id is within the registry and set. */
struct Ent38 { short id; short slot; int ref; };
struct Data38 { char pad[0xBC]; unsigned count; Ent38 *ents; };
struct Self38 { unsigned char active; char pad[3]; Data38 *data; };
extern "C" {
int func_08AA7AD0(Self38 *self, int ref);
int func_08AA9D30(Self38 *self, int slot);
void *func_08899300();
unsigned func_08897EFC(void *reg);
int func_08897998(void *reg, int id);
bool func_08AA8248(Self38 *self, unsigned index)
{
    if (self->active == 0) {
        return false;
    }
    if (index >= self->data->count) {
        return false;
    }
    Ent38 *p = &self->data->ents[index];
    if (func_08AA7AD0(self, p->ref) == 0) {
        return false;
    }
    if (func_08AA9D30(self, p->slot) == 0) {
        return false;
    }
    void *reg = func_08899300();
    unsigned n = func_08897EFC(reg);
    if ((unsigned)(int)p->id < n) {
        if (func_08897998(reg, (int)p->id) == 0) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
}

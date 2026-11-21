/* True when the indexed (id, slot) pair is usable: the slot check passes and the id is within the registry and set. */
struct Pair38 { short id; short slot; };
struct Data38 { char pad[0x2C]; unsigned count; Pair38 *pairs; };
struct Self38 { unsigned char active; char pad[3]; Data38 *data; };
extern "C" {
int func_08AA7AD0(Self38 *self, int slot);
void *func_08899300();
unsigned func_08898194(void *reg);
int func_08897A00(void *reg, unsigned id);
bool func_08AA8184(Self38 *self, unsigned index)
{
    if (self->active == 0) {
        return false;
    }
    if (index >= self->data->count) {
        return false;
    }
    Pair38 *p = &self->data->pairs[index];
    if (func_08AA7AD0(self, p->slot) == 0) {
        return false;
    }
    void *reg = func_08899300();
    unsigned n = func_08898194(reg);
    unsigned id = p->id;
    if (id < n) {
        if (func_08897A00(reg, id) == 0) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
}

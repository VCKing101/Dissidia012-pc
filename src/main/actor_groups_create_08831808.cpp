/* Creates an actor's hit box and its ten hit groups from the body's hit
   table, setting the state bits for the parts that exist. */
struct Body08831808 {
    char pad_00[0x18];
    void *hit_table;
};

struct Actor08831808 {
    char pad_000[0x388];
    unsigned long long state;
    Body08831808 *body;
    char pad_394[0x3A0 - 0x394];
    void *box;
    char pad_3A4[0x468 - 0x3A4];
    void *groups[10];
};

extern "C" {

extern unsigned long long D_089C7F08;
extern unsigned long long D_089C7F10;

void *func_08811EFC(void *table, int kind);
void *func_0883E6E8(Actor08831808 *self, void *table, int group);

void func_08831808(Actor08831808 *self)
{
    if (self->body->hit_table == 0) {
        return;
    }
    self->box = func_08811EFC(self->body->hit_table, 0);
    if (self->box != 0) {
        self->state |= D_089C7F08;
    }
    self->groups[0] = func_0883E6E8(self, self->body->hit_table, 1);
    if (self->groups[0] != 0) {
        self->state |= D_089C7F10;
    }
    self->groups[1] = func_0883E6E8(self, self->body->hit_table, 2);
    self->groups[2] = func_0883E6E8(self, self->body->hit_table, 3);
    self->groups[3] = func_0883E6E8(self, self->body->hit_table, 6);
    self->groups[4] = func_0883E6E8(self, self->body->hit_table, 5);
    self->groups[5] = func_0883E6E8(self, self->body->hit_table, 13);
    self->groups[6] = func_0883E6E8(self, self->body->hit_table, 14);
    self->groups[7] = func_0883E6E8(self, self->body->hit_table, 0);
    self->groups[8] = func_0883E6E8(self, self->body->hit_table, 8);
    self->groups[9] = func_0883E6E8(self, self->body->hit_table, 4);
}

}

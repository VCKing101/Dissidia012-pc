typedef struct Key { float field_00; float field_04; float field_08; float weight; char pad_10[0x10]; } Key;
typedef struct Anim { Key keys[17]; int cur; } Anim;
extern void func_088741C0(Anim *self, Key *key);
void func_08874180(Anim *self)
{
    if (self->keys[self->cur].weight != 0.0f) {
        func_088741C0(self, &self->keys[self->cur]);
    }
}

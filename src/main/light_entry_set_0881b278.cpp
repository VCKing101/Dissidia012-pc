/* Turns on an entry of the light table with a value and two vectors,
   recording newly used entries. */
struct Vec4_0881B278 {
    float x, y, z, w;
};

struct LightEntry0881B278 {
    char pad_00[0xC];
    int used;
    int value;
    char pad_14[0xC];
    Vec4_0881B278 a;
    Vec4_0881B278 b;
};

struct LightTable0881B278 {
    unsigned char ready;
    char pad_01[3];
    unsigned int count;
    LightEntry0881B278 *entries;
    unsigned int *used_list;
    int used_count;
};

extern "C" {

void func_089A3868(Vec4_0881B278 *dst, Vec4_0881B278 *src);

int func_0881B278(LightTable0881B278 *self, unsigned int index, int value, Vec4_0881B278 *a, Vec4_0881B278 *b)
{
    if (!self->ready) {
        return -1;
    }
    if (index >= self->count) {
        return -2;
    }
    if (self->entries[index].used == 0) {
        self->used_list[self->used_count] = index;
        self->used_count++;
    }
    self->entries[index].used = 1;
    self->entries[index].value = value;
    func_089A3868(&self->entries[index].a, a);
    func_089A3868(&self->entries[index].b, b);
    return 0;
}

}

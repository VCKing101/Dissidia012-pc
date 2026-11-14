/* Appends (id, a, b) to a 16-entry ring. */
typedef struct Entry0882F49C {
    int id;
    float a;
    float b;
} Entry0882F49C;

typedef struct Ring0882F49C {
    char pad_00[0x98];
    int index;
    int field_9C;
    Entry0882F49C entries[16];
} Ring0882F49C;

void func_0882F49C(Ring0882F49C *ring, int id, float a, float b)
{
    Entry0882F49C *e = &ring->entries[ring->index];
    ring->index = (ring->index + 1) % 16;
    e->id = id;
    e->a = a;
    e->b = b;
}

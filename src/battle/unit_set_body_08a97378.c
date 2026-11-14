typedef struct BodyLink {
    unsigned char active;
    char pad_01[0x7];
    void *body;
} BodyLink;

void func_08A97378(BodyLink *self, void *body)
{
    if (self->active == 0) {
        return;
    }
    if (body == 0) {
        return;
    }
    self->body = body;
}

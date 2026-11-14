/* World matrix of one of an actor's bones; 0x4000 selects the actor's own
   matrix and the other 0x4000 codes the identity. */
struct Actor088369A4 {
    float matrix[16];
    char pad_040[0x330 - 0x40];
    float world[16];
    char pad_370[0x394 - 0x370];
    void *body;
};

extern "C" {

float *func_0886B590(void *body, unsigned int bone);
void func_089A3EFC(float *dst, Actor088369A4 *src);
void func_089A3EBC(float *dst);
void func_089A3FB8(float *dst, float *a, float *b);

void func_088369A4(Actor088369A4 *self, float *out, unsigned int bone)
{
    if (bone & 0x4000) {
        if (bone == 0x4000) {
            func_089A3EFC(out, self);
        } else {
            func_089A3EBC(out);
        }
    } else {
        float *m = func_0886B590(self->body, bone);
        func_089A3FB8(out, self->world, m);
    }
}

}

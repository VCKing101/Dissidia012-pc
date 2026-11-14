/* Clears an actor's transient action states (and, for the player, the
   guard-break flag). */
struct ActorFlags088341A4 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int dashing : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int jumping : 1;
    unsigned int falling : 1;
    unsigned int landing : 1;
    unsigned int b9 : 1;
    unsigned int blocking : 1;
    unsigned int evading : 1;
    unsigned int stunned : 1;
};

struct Status088341A4 {
    char pad_0[7];
    unsigned char flags;
};

struct Actor088341A4 {
    char pad_000[0x380];
    ActorFlags088341A4 flags;
    char pad_384[0x5B0 - 0x384];
    Status088341A4 status;
};

extern "C" {

int func_0883739C(Actor088341A4 *actor);

void func_088341A4(Actor088341A4 *self)
{
    self->flags.dashing = 0;
    self->flags.jumping = 0;
    self->flags.falling = 0;
    self->flags.landing = 0;
    self->flags.evading = 0;
    self->flags.blocking = 0;
    self->flags.stunned = 0;
    if (func_0883739C(self) == 1) {
        Status088341A4 *s = &self->status;
        s->flags &= ~0x40;
    }
}

}

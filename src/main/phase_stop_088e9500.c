/* Stops the running slot (0x1874) and timer (0x1AF0) when the run flag is set. */
typedef struct PhaseOwner {
    char pad_0000[0x1874];
    char slot[0x1AF0 - 0x1874];
    char timer[0x1B04 - 0x1AF0];
    unsigned char running;
} PhaseOwner;

extern void func_088D6DBC(void *slot);
extern void func_088CE3D8(void *timer);

void func_088E9500(PhaseOwner *owner)
{
    if (owner->running == 0) {
        return;
    }
    func_088D6DBC(owner->slot);
    func_088CE3D8(owner->timer);
    owner->running = 0;
}

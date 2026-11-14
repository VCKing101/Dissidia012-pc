/* Movement speed: the boosted speed while a boost flag is set, otherwise
   the base speed scaled by the global factor. */
struct Unit08838300 {
    char pad_000[0x388];
    unsigned long long flags;
    char pad_390[0x38];
    float base_speed;
    float boost_speed;

    static bool any(Unit08838300 *u, unsigned long long mask) { return (u->flags & mask) != 0; }
};

extern "C" {

extern unsigned long long D_089C7FD8;
extern float D_089D5EAC;

float func_08838300(Unit08838300 *u)
{
    return Unit08838300::any(u, D_089C7FD8) ? u->boost_speed : u->base_speed * D_089D5EAC;
}

}

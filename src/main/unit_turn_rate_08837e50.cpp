/* Sets the turn rate from an angle and the target's speed (sampled again
   when flag 0x80 is set). */
struct Unit08837E50 {
    char pad_000[0x384];
    unsigned int flags;

    static bool has(Unit08837E50 *u, unsigned int mask) { return (u->flags & mask) != 0; }
};

extern "C" {

float func_0884E3AC(void *target);
float func_089A4500(float angle);
void func_089A2C68(float rate);

void func_08837E50(Unit08837E50 *u, void *target, float angle)
{
    float speed = func_0884E3AC(target);
    if (Unit08837E50::has(u, 0x80)) {
        speed = func_0884E3AC(target);
    }
    func_089A2C68(func_089A4500(angle) * 2.0f / speed);
}

}

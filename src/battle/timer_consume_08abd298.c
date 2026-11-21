/* Consumes dt from the remaining time; when it runs out, ends the state and passes the leftover on. */
typedef struct Tm38 { char pad[4]; short state; char pad2[0x56]; float remain; } Tm38;
extern void func_08ABD2E8(float dt);
void func_08ABD298(float time, Tm38 *t)
{
    float remain = t->remain;
    float zero = 0.0f;
    if (time < remain) {
        t->remain = remain - time;
        time = zero;
    } else {
        t->remain = zero;
        time = time - remain;
        t->state = 2;
    }
    func_08ABD2E8(time);
}

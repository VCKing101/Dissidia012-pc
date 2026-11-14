/* Closes the system's handle when it has one, then finishes it. */
typedef struct System088CDF80 {
    int field_00;
    int handle;
} System088CDF80;

void func_0899BB1C(int handle);
void func_088CDF70(System088CDF80 *system);

void func_088CDF80(System088CDF80 *system)
{
    if (system->handle != 0) {
        func_0899BB1C(system->handle);
    }
    func_088CDF70(system);
}

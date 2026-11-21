/* Checks the entry's index against the manager's count; clears it to -1 when the lookup fails. */
typedef struct Ent38 { unsigned char active; char pad[0x13]; unsigned index; } Ent38;
typedef struct Mgr38 { unsigned char active; char pad[0xBB]; unsigned count; } Mgr38;
extern Mgr38 *func_08AA79C4(void *p);
extern int func_08AA8248(void *p, unsigned index);
extern void *D_08AAAD80;
int func_08A9452C(Ent38 *self)
{
    void *owner;
    Mgr38 *mgr;
    if (self->active == 0) {
        return -1;
    } else {
        owner = D_08AAAD80;
        mgr = func_08AA79C4(owner);
        if (mgr->active == 0) {
            return 0;
        } else {
            if (self->index < mgr->count) {
                if (func_08AA8248(owner, self->index) == 0) {
                    self->index = -1;
                }
                return 0;
            } else {
                return 0;
            }
        }
    }
}

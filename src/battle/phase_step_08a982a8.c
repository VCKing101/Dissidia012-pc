/* Runs the handler for the object's current phase (0..2), then the common step; negative codes name the failing stage. */
typedef struct Obj38 { unsigned char active; char pad[3]; int phase; } Obj38;
extern int func_08899944();
extern int func_08A98FC4(Obj38 *o, int a, int b, int c, int d, int e);
extern int func_08A99558(Obj38 *o);
extern int func_08A99CF0(Obj38 *o, int a);
extern int func_08A99290(Obj38 *o, int a);
int func_08A982A8(Obj38 *o, int a, int b, int c, int d, int e)
{
    int p;
    if (o->active == 0) {
        return -1;
    } else {
        if (func_08899944() == 1) {
            o->phase = 0;
            return 0;
        } else {
            p = o->phase;
            switch (p) {
            case 0:
                if (func_08A98FC4(o, a, b, c, d, e) != 0) {
                    return -2;
                }
                break;
            case 1:
                if (func_08A99558(o) != 0) {
                    return -3;
                }
                break;
            case 2:
                if (func_08A99CF0(o, a) != 0) {
                    return -4;
                }
                break;
            }
            if (func_08A99290(o, a) != 0) {
                return -5;
            } else {
                return 0;
            }
        }
    }
}

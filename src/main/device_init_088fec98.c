/* Brings the device up (unless asked not to): three steps in order, each logged and its error code returned on failure; returns 0 on success. */
extern char D_089CDA18[];
extern void func_089A7DE8(char *self);
extern int func_089A77C0(char *self, char *p, int a, int b);
extern int func_089A78E0(char *self);
extern int func_089A7AB0(char *self, int a, int b);
extern void func_089AAB08(char *fmt, char *arg);
int func_088FEC98(char *self, unsigned char skip)
{
    int r;
    func_089A7DE8(self);
    if (skip == 0) {
        r = func_089A77C0(self, self + 0x400, 0, 0);
        if (r >= 0) {
            r = func_089A78E0(self);
            if (r >= 0) {
                r = func_089A7AB0(self, 0, 0);
                if (r >= 0) {
                } else {
                    func_089AAB08(D_089CDA18, self + 0x1EC);
                    return r;
                }
            } else {
                func_089AAB08(D_089CDA18, self + 0x1EC);
                return r;
            }
        } else {
            func_089AAB08(D_089CDA18, self + 0x1EC);
            return r;
        }
    } else {
        func_089AAB08(D_089CDA18, self + 0x1EC);
        return 0;
    }
    return 0;
}

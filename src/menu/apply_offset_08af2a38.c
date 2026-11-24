/* When D_09B6CDA0+0xE8 is not -1 and kind is 4, 1 or 7, stores max(a1 - that, 0) into a record found via func_08AF9720 and clears the marker. */
typedef struct { char p0[8]; int sel; int tbl[1]; } Hd;
typedef struct { char p0[0xE8 - 0xC]; int mark; } Dummy;
typedef struct { char p0[8]; int sel; int tbl[(0xE8 - 0xC) / 4]; int mark; } Glob;
typedef struct { char p0[0xC]; int v; } Rec;
extern Glob D_09B6CDA0;
extern void func_088CB9E8(void *buf);
extern Rec *func_08AF9720(void *buf, int a);
void func_08AF2A38(int a0, int a1, int kind) {
    char buf[1912];
    int v;
    if (D_09B6CDA0.mark != -1) {
        if (kind == 4 || kind == 1 || kind == 7) {
            v = a1 - D_09B6CDA0.mark;
            if (v < 0) {
                v = 0;
            }
            func_088CB9E8(buf);
            func_08AF9720(buf, D_09B6CDA0.tbl[D_09B6CDA0.sel])->v = v;
            D_09B6CDA0.mark = -1;
        }
    }
}

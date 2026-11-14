typedef struct Agent4KindInfo {
    int field_00;
    unsigned char kind;
} Agent4KindInfo;

typedef struct Agent4KindSource {
    unsigned char unknown_0000[0x4C];
    Agent4KindInfo *info;
} Agent4KindSource;

void func_08829930(void);
void func_0882CEC0(void);
void func_08828EEC(float scale, int target, unsigned char kind, int a, int b, int c, int d,
                   void (*callback)(void), Agent4KindSource *source);
void func_08829104(float scale, int target, unsigned char kind, int a, int b, int c, int d,
                   void (*callback)(void), Agent4KindSource *source);

void func_08828E94(float scale, int target, Agent4KindSource *source, int a, int b, int c, int d) {
    func_08828EEC(scale, target, source->info->kind, a, b, c, d, func_08829930, source);
}

void func_08828EC0(float scale, int target, Agent4KindSource *source, int a, int b, int c, int d) {
    func_08829104(scale, target, source->info->kind, a, b, c, d, func_0882CEC0, source);
}

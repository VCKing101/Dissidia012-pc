struct Scene08893558 {
    unsigned char unknown_00[0x74];
    int active;
};

extern "C" {
extern Scene08893558 *D_089D6AC8;
int *func_08899300(void);
int func_08899944(void);
void func_088922C8(Scene08893558 *scene);
void func_08893DAC(Scene08893558 *scene);
}

struct Check08893558 {
    static bool set(int *value) {
        return *value != 0;
    }
};

extern "C" {
void func_08893558(void) {
    Scene08893558 *scene = D_089D6AC8;
    if (scene->active != 0) {
        if (Check08893558::set(func_08899300()) && func_08899944() == 0) {
            func_08899300();
        }
        func_088922C8(scene);
        func_08893DAC(scene);
    }
}
}

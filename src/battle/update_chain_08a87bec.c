typedef struct Bt { char p0; unsigned char on; } Bt;
extern int func_08A8A530(void *, int, void *);
extern int func_08A90170(void *, void *);
extern int func_08A929DC(void *, void *);
extern int func_08A90FC8(void *, void *);
extern int func_08A91BA0(void *, void *);
extern int func_08A91F94(void *, void *);
extern int func_08A91660(void *, void *);
extern int func_08A92480(void *, void *);
extern int func_08A8F8CC(void *, void *);
extern int func_08A8F134(void *, void *);
extern int func_08A9082C(void *, void *);
extern int func_08A898D0(void *, void *);
extern int func_08A89EE0(void *, void *);
extern int func_08A88C84(void *, void *);
extern int func_08A8931C(void *, void *);

int func_08A87BEC(Bt *self, int a1) {
    int r;
    if (self->on == 0) {
        return 0;
    } else {
        r = func_08A8A530((char *)self + 0x230, a1, self);
        if (r == 0) {
            r = func_08A90170((char *)self + 0x450, self);
            if (r == 0) {
                r = func_08A929DC((char *)self + 0x470, self);
                if (r == 0) {
                    r = func_08A90FC8((char *)self + 0x4A0, self);
                    if (r == 0) {
                        r = func_08A91BA0((char *)self + 0x4C0, self);
                        if (r == 0) {
                            r = func_08A91F94((char *)self + 0x4E0, self);
                            if (r == 0) {
                                r = func_08A91660((char *)self + 0x510, self);
                                if (r == 0) {
                                    r = func_08A92480((char *)self + 0x530, self);
                                    if (r == 0) {
                                        r = func_08A8F8CC((char *)self + 0x550, self);
                                        if (r == 0) {
                                            r = func_08A8F134((char *)self + 0x590, self);
                                            if (r == 0) {
                                                r = func_08A9082C((char *)self + 0x5B0, self);
                                                if (r == 0) {
                                                    r = func_08A898D0((char *)self + 0x600, self);
                                                    if (r == 0) {
                                                        r = func_08A89EE0((char *)self + 0x61C, self);
                                                        if (r == 0) {
                                                            r = func_08A88C84((char *)self + 0x62C, self);
                                                            if (r == 0) {
                                                                r = func_08A8931C((char *)self + 0x648, self);
                                                                if (r != 0) {
                                                                    return -15;
                                                                } else {
                                                                    return 0;
                                                                }
                                                            } else {
                                                                return -14;
                                                            }
                                                        } else {
                                                            return -13;
                                                        }
                                                    } else {
                                                        return -12;
                                                    }
                                                } else {
                                                    return -11;
                                                }
                                            } else {
                                                return -10;
                                            }
                                        } else {
                                            return -9;
                                        }
                                    } else {
                                        return -8;
                                    }
                                } else {
                                    return -7;
                                }
                            } else {
                                return -6;
                            }
                        } else {
                            return -5;
                        }
                    } else {
                        return -4;
                    }
                } else {
                    return -3;
                }
            } else {
                return -2;
            }
        } else {
            return -1;
        }
    }
}

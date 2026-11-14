typedef struct PackageInfo { int field_00; int field_04; int field_08; } PackageInfo;
extern PackageInfo D_09B26BC0;
int func_08871674(void)
{
    PackageInfo *info = &D_09B26BC0;
    return info->field_08;
}

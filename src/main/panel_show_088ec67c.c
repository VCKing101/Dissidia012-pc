/* Shows the panel's widget and marks both shown. */
typedef struct PanelWidget {
    char pad_00[0xE];
    short visible;
} PanelWidget;

typedef struct Panel {
    int field_00;
    PanelWidget *widget;
    int shown;
} Panel;

extern void func_088D9F5C(PanelWidget *widget, int arg, unsigned char on);

void func_088EC67C(Panel *panel)
{
    func_088D9F5C(panel->widget, 0, 1);
    panel->widget->visible = 1;
    panel->shown = 1;
}

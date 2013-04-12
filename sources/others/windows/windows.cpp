#include "../../includes/others/windows/windows.h"

Windows::Windows(QWidget* w)
{
    if (w == NULL)
    {
        return;
    }
    w->winId();
}

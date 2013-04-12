#include "../../includes/others/mac/mac.h"

#include "AppKit.h"

Mac::Mac(QWidget* w)
{
    if (w == NULL)
    {
        return;
    }
    mParent = w;

    NSView *nsview = (NSView *)mParent->winId();
    NSWindow *nswindow = [nsview window];
    [nswindow setHasShadow:NO];
    [nswindow setStyleMask:NSBorderlessWindowMask | NSResizableWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask];

    if (QSysInfo::MacintoshVersion >= QSysInfo::MV_10_7)
    {
        [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    }
}

void Mac::enterFullscreenMode()
{
    if (mParent == NULL) return;

    NSView *nsview = (NSView *)mParent->winId();
    NSWindow *nswindow = [nsview window];
    [nswindow toggleFullScreen:nil];
}

void Mac::registerAsDefaultBrowser()
{
    LSSetDefaultHandlerForURLScheme(CFSTR("http"), (CFStringRef)@"com.dviance.Helm");
    LSSetDefaultHandlerForURLScheme(CFSTR("https"), (CFStringRef)@"com.dviance.Helm");
    LSSetDefaultRoleHandlerForContentType(kUTTypeHTML, kLSRolesViewer, (CFStringRef)@"com.dviance.Helm");
    LSSetDefaultRoleHandlerForContentType(kUTTypeURL, kLSRolesViewer, (CFStringRef)@"com.dviance.Helm");
}

#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#ifdef __APPLE__
#include <objc/objc-runtime.h>
#else
#include <gtk/gtk.h>
#endif
#endif

#ifdef __APPLE__
NSString* NSStringFrom(const std::string& input) {
    return objc_msgSend((id)objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"), input.c_str());
}
#endif

void ShowMessageBox(const std::string& title, const std::string& message) {
#ifdef _WIN32
    MessageBox(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
#elif __APPLE__
    id pool = objc_msgSend((id)objc_getClass("NSAutoreleasePool"), sel_registerName("new"));
    id alert = objc_msgSend((id)objc_getClass("NSAlert"), sel_registerName("new"));
    objc_msgSend(alert, sel_registerName("setMessageText:"), NSStringFrom(message));
    objc_msgSend(alert, sel_registerName("setInformativeText:"), NSStringFrom(title));
    objc_msgSend(alert, sel_registerName("addButtonWithTitle:"), NSStringFrom("OK"));
    objc_msgSend(alert, sel_registerName("runModal"));
    objc_msgSend(alert, sel_registerName("release"));
    objc_msgSend(pool, sel_registerName("drain"));
#else // Assumes Linux
    gtk_init(0, NULL);
    GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message.c_str());
    gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_main_quit();
#endif
}
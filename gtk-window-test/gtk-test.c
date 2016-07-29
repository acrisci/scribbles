#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init (&argc, &argv);
    GtkWidget *widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect (G_OBJECT (widget), "destroy", G_CALLBACK (gtk_main_quit), NULL);  

    // test things
    gtk_window_set_accept_focus((GtkWindow*)widget, FALSE);
    gtk_window_set_type_hint((GtkWindow*)widget, GDK_WINDOW_TYPE_HINT_TOOLBAR);

    gtk_widget_show(widget);
    gtk_main();
    return 0;
}

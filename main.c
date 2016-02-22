#include "main.h"

static gboolean on_draw_event(GtkWidget *widget, cairo_t *window_cr,
                              gpointer data) {
  return FALSE;
}

int main(int argc, char **argv) {
  GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Color Supplier");

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), 256, 256);

  GtkWidget *da;
  da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, 256, 256);
  g_signal_connect(da, "draw", G_CALLBACK(on_draw_event), NULL);

  gtk_container_add(GTK_CONTAINER(window), da);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

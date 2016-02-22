#include <cairo.h>
#include <gtk/gtk.h>

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data);
int determine_subset(int x, int max, int subsets);

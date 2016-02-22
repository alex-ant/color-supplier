#include <cairo.h>
#include <gtk/gtk.h>

struct colors {
   int r;
   int g;
   int b;
};

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data);
int determine_subset(int x, int max);
struct colors get_colors(int x, int max);

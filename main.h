#include <cairo.h>
#include <gtk/gtk.h>

struct colors {
  int r;
  int g;
  int b;
};

typedef struct {
  int start;
  int end;
} subset;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data);
int determine_subset(int x, int max);
struct colors get_colors(int x, int max);
struct colors set_hue(int y, int max, struct colors subset_colors);
void init_subsets();

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

static gboolean on_da_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data);
static gboolean on_press_event(GtkWidget *widget, GdkEventButton *event,
                               gpointer data);
static gboolean on_release_event(GtkWidget *widget, GdkEventButton *event,
                                 gpointer data);
static gboolean on_motion_event(GtkWidget *widget, GdkEventMotion *event,
                                gpointer data);
static gboolean on_status_draw_event(GtkWidget *widget, cairo_t *cr,
                                     gpointer data);

int determine_subset(int x, int max);
struct colors get_colors(int x, int max);
struct colors set_hue(int y, struct colors subset_colors);
void init_subsets();
void print_coordinate_colors(int x, int y);

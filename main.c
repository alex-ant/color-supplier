#include "main.h"
#include <stdio.h>

int width = 800;
int height = 300;

const int SUBSETS = 5;

int determine_subset(int x, int max) {
  int i;
  for (i = 1; i <= SUBSETS; i++) {
    if (x >= max*(i-1)/SUBSETS && x <= max*i/SUBSETS) {
      return i;
    }
  }
  return 0;
}

struct colors get_colors(int x, int max) {
  struct colors subset_colors;
  subset_colors.r = 0;
  subset_colors.g = 0;
  subset_colors.b = 0;

  int subset = determine_subset(x, max);
  int subset_size = max/SUBSETS;

  int color_start, color_start2, delta, delta2;

  switch (subset) {
      case 1:
      color_start = -subset_size;
      delta = x - color_start;

      subset_colors.r = 0;
      subset_colors.g = 0;
      subset_colors.b = delta*255/(subset_size*2);
      break;

      case 2:
      color_start = subset_size;
      delta = x - color_start;

      subset_colors.r = 0;
      subset_colors.g = delta*255/subset_size;
      subset_colors.b = 255;
      break;

      case 3:
      color_start = subset_size*2;
      delta = x - color_start;

      color_start2 = subset_size*2;
      delta2 = x - color_start2;

      subset_colors.r = delta2*255/subset_size;
      subset_colors.g = 255;
      subset_colors.b = (subset_size - delta)*255/subset_size;
      break;

      case 4:
      color_start = subset_size*3;
      delta = x - color_start;

      subset_colors.r = 255;
      subset_colors.g = (subset_size - delta)*255/subset_size;
      subset_colors.b = 0;
      break;

      case 5:
      color_start = subset_size*4;
      delta = x - color_start;

      subset_colors.r = (subset_size*2 - delta)*255/(subset_size*2);
      subset_colors.g = 0;
      subset_colors.b = 0;
      break;
  }

  return subset_colors;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
  cairo_set_line_width(cr, 1);

  double max = 255;
  int r, g, b, x, y;
  for (x = 0; x < width; x++) {
    for (y = 0; y < width; y++) {
      struct colors c = get_colors(x, width);
      cairo_set_source_rgb(cr, (double)c.r / max, (double)c.g / max, (double)c.b / max);
      cairo_rectangle(cr, x, y, 1, 1);
      cairo_fill(cr);
    }
  }

  return FALSE;
}

int main(int argc, char **argv) {
  GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Color Supplier");

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), width, 512);

  GtkWidget *da;
  da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, width, 512);
  g_signal_connect(da, "draw", G_CALLBACK(on_draw_event), NULL);

  gtk_container_add(GTK_CONTAINER(window), da);

  gtk_widget_show_all(window);

  gtk_widget_queue_draw(da);

  gtk_main();

  return 0;
}

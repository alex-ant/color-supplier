#include "main.h"
#include <stdio.h>

int width = 512;
int height = 512;

const int SUBSETS = 7;

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

  int subset = determine_subset(x, max);
  int color1_size, color2_size, x_delta1, x_delta2;

  switch (subset) {
    case 1:
      // R
      color1_size = 2;

      subset_colors.r = x*255/color1_size;
      subset_colors.g = 0;
      subset_colors.b = 0;
    break;
    case 2:
      // RG
      color1_size = 2;
      color2_size = 3;

      //x_delta1 = x-max/subset;

      subset_colors.r = 255;
      subset_colors.g = 255;
      subset_colors.b = 0;
    break;
    case 3:
      // G
      subset_colors.r = 0;
      subset_colors.g = 255;
      subset_colors.b = 0;
    break;
    case 4:
      // GB
      subset_colors.r = 0;
      subset_colors.g = 255;
      subset_colors.b = 255;
    break;
    case 5:
      // B
      subset_colors.r = 0;
      subset_colors.g = 0;
      subset_colors.b = 255;
    break;
    case 6:
      // BR
      subset_colors.r = 255;
      subset_colors.g = 0;
      subset_colors.b = 255;
    break;
    case 7:
      // R
      subset_colors.r = 255;
      subset_colors.g = 0;
      subset_colors.b = 0;
    break;
  }

  return subset_colors;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
  cairo_set_line_width(cr, 1);

  /*int r, g, b, x, y;
  double max = 255;
  for (r = 0; r < 256; r++) {
    for (g = 0; g < 256; g++) {
      for (b = 0; b < 256; b++) {
        cairo_set_source_rgb(cr, (double)r / max, (double)g / max,
                             (double)b / max);
        cairo_rectangle(cr, x, y, 1, 1);
        cairo_fill(cr);
      }
      y++;
    }
    x++;
  }*/

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
  /*struct colors c = get_colors(430, 512);
  printf("===>> %d, %d, %d\n", c.r, c.g, c.b);
  return 0;*/

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

#include "main.h"
#include <stdio.h>

int width = 800;
int height = 300;

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
  subset_colors.r = 0;
  subset_colors.g = 0;
  subset_colors.b = 0;

  int color_val, color2_val;

  int subset = determine_subset(x, max);
  int subset_size = max/SUBSETS;
  int subset_start = max/SUBSETS*(subset-1);
  int color_mid = subset_size*3/2;

  if (subset%2 != 0) {
    // pure colors
    int color_start = subset_start - subset_size;
    int delta = x - color_start;
    if (delta <= color_mid) {
      color_val = delta*255/color_mid;
    } else {
      color_val = (subset_size*3 - delta)*255/color_mid;
    }

    switch (subset) {
        case 1:
        // R
        subset_colors.r = color_val;
        break;

        case 3:
        // G
        subset_colors.g = color_val;
        break;

        case 5:
        // B
        subset_colors.b = color_val;
        break;

        case 7:
        // R
        subset_colors.r = color_val;
        break;
    }

  } else {
    // mixed colors
    int color1_start = subset_start - subset_size*2;
    int delta1 = x - color1_start;
    if (delta1 <= color_mid) {
      color_val = delta1*255/color_mid;
    } else {
      color_val = (subset_size*3 - delta1)*255/color_mid;
    }

    int color2_start = subset_start;
    int delta2 = x - color2_start;
    if (delta2 <= color_mid) {
      color2_val = delta2*255/color_mid;
    } else {
      color2_val = (subset_size*3 - delta2)*255/color_mid;
    }

    switch (subset) {
        case 2:
        // RG
        subset_colors.r = color_val;
        subset_colors.g = color2_val;
        break;

        case 4:
        // GB
        subset_colors.g = color_val;
        subset_colors.b = color2_val;
        break;

        case 6:
        // BR
        subset_colors.r = color2_val;
        subset_colors.b = color_val;
        break;
    }
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

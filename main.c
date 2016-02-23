#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int width = 800;
int height = 512;

const int SUBSETS = 5;
subset subsets[5];

int determine_subset(int x, int max) {
  int i;
  for (i = 0; i < SUBSETS; i++) {
    if (x >= subsets[i].start && x <= subsets[i].end) {
      return i;
    }
  }
  return 0;
}

void init_subsets() {
  double borders[] = {0.12, 0.34, 0.66, 0.84, 1};
  subset s;
  int i;
  for (i = 0; i < SUBSETS; i++) {
    if (i == 0) {
      s.start = 0;
    } else {
      s.start = subsets[i - 1].end + 1;
    }
    s.end = width * borders[i];
    subsets[i] = s;
  }
}

struct colors get_colors(int x, int max) {
  struct colors subset_colors;
  subset_colors.r = 0;
  subset_colors.g = 0;
  subset_colors.b = 0;

  int subset = determine_subset(x, max);
  int subset_size = subsets[subset].end - subsets[subset].start;

  int color_start, color_start2, delta, delta2;

  switch (subset) {
  case 0:
    color_start = -subset_size;
    delta = x - color_start;

    subset_colors.r = delta * 255 / (subset_size * 2);
    break;

  case 1:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    subset_colors.r = 255;
    subset_colors.g = delta * 255 / subset_size;
    break;

  case 2:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    color_start2 = subsets[subset - 1].end;
    delta2 = x - color_start2;

    subset_colors.r = (subset_size - delta) * 255 / subset_size;
    subset_colors.g = 255;
    subset_colors.b = delta2 * 255 / subset_size;
    break;

  case 3:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    subset_colors.g = (subset_size - delta) * 255 / subset_size;
    subset_colors.b = 255;
    break;

  case 4:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    subset_colors.b = (subset_size * 2 - delta) * 255 / (subset_size * 2);
    break;
  }

  return subset_colors;
}

struct colors set_hue(int y, int max, struct colors subset_colors) {
  struct colors new_subset_colors;

  int middle = max / 2;
  int delta = y - middle;
  if (delta <= 0) {
    new_subset_colors.r =
        subset_colors.r - (delta * (255 - subset_colors.r) / middle);
    new_subset_colors.g =
        subset_colors.g - (delta * (255 - subset_colors.g) / middle);
    new_subset_colors.b =
        subset_colors.b - (delta * (255 - subset_colors.b) / middle);
  } else {
    new_subset_colors.r = (max - y) * subset_colors.r / middle;
    new_subset_colors.g = (max - y) * subset_colors.g / middle;
    new_subset_colors.b = (max - y) * subset_colors.b / middle;
  }

  return new_subset_colors;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
  cairo_set_line_width(cr, 1);

  double max = 255;
  int x, y;
  struct colors cx, cy;
  for (x = 0; x < width; x++) {
    cx = get_colors(x, width);
    for (y = 0; y < height; y++) {
      cy = set_hue(y, height, cx);
      cairo_set_source_rgb(cr, (double)cy.r / max, (double)cy.g / max,
                           (double)cy.b / max);
      cairo_rectangle(cr, x, y, 1, 1);
      cairo_fill(cr);
    }
  }

  return FALSE;
}

int main(int argc, char **argv) {
  if (argc == 3) {
    width = atoi(argv[1]);
    height = atoi(argv[2]);
  }

  init_subsets();

  GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Color Supplier");

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), width, height);

  GtkWidget *da;
  da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, width, height);
  g_signal_connect(da, "draw", G_CALLBACK(on_draw_event), NULL);

  gtk_container_add(GTK_CONTAINER(window), da);

  gtk_widget_show_all(window);

  gtk_widget_queue_draw(da);

  gtk_main();

  return 0;
}

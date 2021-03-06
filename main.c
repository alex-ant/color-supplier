#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int width = 800;
int height = 512;
int status_height = 20;

GtkWidget *da;
GtkWidget *status_bar;

struct colors active_color;

const int SUBSETS = 8;
subset subsets[8];

int dragging = 0;

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
  double borders[] = {0.08, 0.25, 0.42, 0.5, 0.67, 0.83, 0.92, 1};
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
    subset_colors.r = 255;
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

    subset_colors.r = (subset_size - delta) * 255 / subset_size;
    subset_colors.g = 255;
    break;

  case 3:
    subset_colors.g = 255;
    break;

  case 4:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    subset_colors.g = 255;
    subset_colors.b = delta * 255 / subset_size;
    break;

  case 5:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    subset_colors.g = (subset_size - delta) * 255 / subset_size;
    subset_colors.b = 255;
    break;

  case 6:
    subset_colors.b = 255;
    break;

  case 7:
    color_start = subsets[subset - 1].end;
    delta = x - color_start;

    subset_colors.r = delta * 255 / subset_size;
    subset_colors.b = 255;
    break;
  }

  return subset_colors;
}

struct colors set_hue(int y, struct colors subset_colors) {
  struct colors new_subset_colors;

  if (y <= height / 2) {
    new_subset_colors.r =
        (2 * subset_colors.r * y + 255 * height - 510 * y) / height;
    new_subset_colors.g =
        (2 * subset_colors.g * y + 255 * height - 510 * y) / height;
    new_subset_colors.b =
        (2 * subset_colors.b * y + 255 * height - 510 * y) / height;
  } else {
    new_subset_colors.r = (2 * subset_colors.r * (height - y)) / height;
    new_subset_colors.g = (2 * subset_colors.g * (height - y)) / height;
    new_subset_colors.b = (2 * subset_colors.b * (height - y)) / height;
  }

  return new_subset_colors;
}

static gboolean on_da_draw_event(GtkWidget *widget, cairo_t *cr,
                                 gpointer data) {
  cairo_set_line_width(cr, 1);

  double max = 255;
  int x, y;
  struct colors cx, cy;
  for (x = 0; x < width; x++) {
    cx = get_colors(x, width);
    for (y = 0; y < height; y++) {
      cy = set_hue(y, cx);
      cairo_set_source_rgb(cr, (double)cy.r / max, (double)cy.g / max,
                           (double)cy.b / max);
      cairo_rectangle(cr, x, y, 1, 1);
      cairo_fill(cr);
    }
  }

  return FALSE;
}

static gboolean on_status_draw_event(GtkWidget *widget, cairo_t *cr,
                                     gpointer data) {
  double max = 255;
  cairo_set_source_rgb(cr, (double)active_color.r / max,
                       (double)active_color.g / max,
                       (double)active_color.b / max);
  cairo_paint(cr);

  return FALSE;
}

void print_coordinate_colors(int x, int y) {
  if (x >= 0 && y >= 0 && x < width && y < height) {
    active_color = set_hue(y, get_colors(x, width));
    gtk_widget_queue_draw(status_bar);
    printf("%d,%d,%d\n", active_color.r, active_color.g, active_color.b);
  } else {
    dragging = 0;
  }
}

static gboolean on_press_event(GtkWidget *widget, GdkEventButton *event,
                               gpointer data) {
  if (event->button == GDK_BUTTON_PRIMARY) {
    dragging = 1;
    print_coordinate_colors((int)event->x, (int)event->y);
  }
  return TRUE;
}

static gboolean on_release_event(GtkWidget *widget, GdkEventButton *event,
                                 gpointer data) {
  if (event->button == GDK_BUTTON_PRIMARY) {
    dragging = 0;
  }
  return TRUE;
}

static gboolean on_motion_event(GtkWidget *widget, GdkEventMotion *event,
                                gpointer data) {
  if (dragging == 1) {
    print_coordinate_colors((int)event->x, (int)event->y);
  }
  return TRUE;
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

  gtk_window_set_default_size(GTK_WINDOW(window), width,
                              height + status_height);

  da = gtk_drawing_area_new();
  gtk_widget_set_size_request(da, width, height);

  g_signal_connect(da, "draw", G_CALLBACK(on_da_draw_event), NULL);
  g_signal_connect(da, "button-press-event", G_CALLBACK(on_press_event), NULL);
  g_signal_connect(da, "button-release-event", G_CALLBACK(on_release_event),
                   NULL);
  g_signal_connect(da, "motion-notify-event", G_CALLBACK(on_motion_event),
                   NULL);

  gtk_widget_set_events(da, gtk_widget_get_events(da) | GDK_BUTTON_PRESS_MASK |
                                GDK_BUTTON_RELEASE_MASK |
                                GDK_POINTER_MOTION_MASK);

  status_bar = gtk_drawing_area_new();
  gtk_widget_set_size_request(status_bar, width, status_height);
  g_signal_connect(status_bar, "draw", G_CALLBACK(on_status_draw_event), NULL);

  GtkWidget *fixed;
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);

  gtk_fixed_put(GTK_FIXED(fixed), da, 0, 0);
  gtk_fixed_put(GTK_FIXED(fixed), status_bar, 0, height);

  gtk_widget_show_all(window);

  gtk_widget_queue_draw(da);

  gtk_main();

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "E97-asm.h"
//#include <gtksourceview/gtksource.h>

static GtkWidget *lampanelInput;
static GtkWidget *r0;
static GtkWidget *r1;
static GtkWidget *r2;
static GtkWidget *r3;

static void runE97(){
  GtkTextIter start, end;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(lampanelInput);
  char *text;

  gtk_text_buffer_get_bounds (buffer, &start, &end);
  text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

  printf(text);

  extern runE97(text);

  g_free (text);
}

static void windowActivate(GApplication *app){
  GtkWidget *window, *box1, *box2, *box3, *runProgram, *titlebar;
  GdkPixbuf *icon;

  // Play Button
  runProgram = gtk_button_new_from_icon_name("media-playback-start-symbolic");
  g_signal_connect(runProgram, "clicked", G_CALLBACK(runE97), NULL);

  // Titlebar [>    -^x]
  titlebar = gtk_header_bar_new();
  gtk_header_bar_pack_start(GTK_HEADER_BAR(titlebar), GTK_WIDGET(runProgram));

  // Window
  window = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(window), "Lampanel GTK");
  gtk_window_set_icon_name(GTK_WINDOW(window), "application-x-executable");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  gtk_window_set_titlebar(GTK_WINDOW(window), titlebar);

  // Horizontal Main Box
  box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);
  gtk_window_set_child(GTK_WINDOW(window), box1);

  // Vertical Child Box (Right)
  box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box3), TRUE);
  gtk_box_append(GTK_BOX(box1), box3);

  // Code Input Field
  lampanelInput = gtk_text_view_new();
  gtk_box_append(GTK_BOX(box3), lampanelInput);

  // Vertical Child Box (Left)
  box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);
  gtk_box_append(GTK_BOX(box1), box2);

  // Registors
  r0 = gtk_label_new("R0: 0000 0000 0000 0000");
  r1 = gtk_label_new("R0: 0000 0000 0000 0000");
  r2 = gtk_label_new("R0: 0000 0000 0000 0000");
  r3 = gtk_label_new("R0: 0000 0000 0000 0000");
  gtk_box_append(GTK_BOX(box2), r0);
  gtk_box_append(GTK_BOX(box2), r1);
  gtk_box_append(GTK_BOX(box2), r2);
  gtk_box_append(GTK_BOX(box2), r3);

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv){
  GtkApplication *window;
  int status;

  window = gtk_application_new("degroland.den0620.lampanel-gtk", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(window, "activate", G_CALLBACK(windowActivate), NULL);
  status = g_application_run(G_APPLICATION(window), argc, argv);
  g_object_unref(window);

  return status;
}


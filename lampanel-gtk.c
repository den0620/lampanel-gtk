#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdint.h> // see E97-asm.h
#include "E97-asm.h"
//#include <gtksourceview/gtksource.h>

static GtkWidget *lampanelInput;
static GtkWidget *r0, *r1, *r2, *r3;
static GtkWidget *exitStatus;
static uint16_t lampsMatrix[8]; // Actually not a matrix, int 0000 0000 0000 0000 x8 rows


static void generateLamps(GtkWidget *verticalRows){
  GtkWidget *horizontalRows, *lampImage;

  for(int i=0;i<8;i++){
    horizontalRows = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_set_homogeneous(GTK_BOX(horizontalRows), TRUE);
    gtk_box_append(GTK_BOX(verticalRows), horizontalRows);

    for(int j=0;j<16;j++){
      if((lampsMatrix[i] >> (15-j))%2 == 0){ // Lamp is off
        lampImage = gtk_image_new_from_file("./icons/off.png");
      } else { // Lamp is on
        lampImage = gtk_image_new_from_file("./icons/on.png");
      }
      gtk_box_append(GTK_BOX(horizontalRows),lampImage);
    }
  }

}

static void runEmulation(){
  char *text;
  int64_t registors;
  GtkTextIter start, end;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(lampanelInput);

  gtk_text_buffer_get_bounds (buffer, &start, &end);
  text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

  printf(text);

  registors = runE97(text);

  // unglue 64 bit int to 4 16 bit registors and print 'em

  // code needed

  g_free (text);
}

static void windowActivate(GApplication *app){
  GtkWidget *window, *box1, *box2, *box3, *runProgram, *titlebar, *verticalRows;
  GdkPixbuf *icon;

  // Play Button
  runProgram = gtk_button_new_from_icon_name("media-playback-start-symbolic");
  g_signal_connect(runProgram, "clicked", G_CALLBACK(runEmulation), NULL);

  // Titlebar [>    -^x]
  titlebar = gtk_header_bar_new();
  gtk_header_bar_pack_start(GTK_HEADER_BAR(titlebar), GTK_WIDGET(runProgram));

  // Window
  window = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(window), "Lampanel GTK");
  gtk_window_set_icon_name(GTK_WINDOW(window), "application-x-executable");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_titlebar(GTK_WINDOW(window), titlebar);

  // Horizontal Main Box
  box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);
  gtk_window_set_child(GTK_WINDOW(window), box1);

  // Vertical Child Box (Right)
  box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box3), TRUE);
  gtk_box_append(GTK_BOX(box1), box3);

  // Lamps field
  verticalRows = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
  gtk_box_set_homogeneous(GTK_BOX(verticalRows), TRUE);
  gtk_box_append(GTK_BOX(box3), verticalRows);
  generateLamps(verticalRows);

  // Code Input Field
  lampanelInput = gtk_text_view_new();
  gtk_box_append(GTK_BOX(box3), lampanelInput);

  // Vertical Child Box (Left)
  box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);
  gtk_box_append(GTK_BOX(box1), box2);

  // Registors
  r0 = gtk_label_new("R0: 0000 0000 0000 0000");
  r1 = gtk_label_new("R1: 0000 0000 0000 0000");
  r2 = gtk_label_new("R2: 0000 0000 0000 0000");
  r3 = gtk_label_new("R3: 0000 0000 0000 0000");
  exitStatus = gtk_label_new("Status:");
  gtk_box_append(GTK_BOX(box2), r0);
  gtk_box_append(GTK_BOX(box2), r1);
  gtk_box_append(GTK_BOX(box2), r2);
  gtk_box_append(GTK_BOX(box2), r3);
  gtk_box_append(GTK_BOX(box2), exitStatus);

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


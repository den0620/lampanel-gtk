#include <gtk/gtk.h>
#include <stdint.h> // see E97-asm.h
#include "E97-asm.h"
//#include <gtksourceview/gtksource.h>

static GtkWidget *lampanelInput;
static GtkWidget *r0, *r1, *r2, *r3;
static GtkWidget *exitStatus;
static GtkWidget *lampsBox;
static GtkWidget *regsBox;
static uint16_t lampsMatrix[8]; // Actually not a matrix, int 0000 0000 0000 0000 x8 rows


static void generateLamps(){
  GtkWidget *lampImage;
  // Lamps changing by lampsMatrix like this: (8x3 example)
  // [ 0001_1100,     ooo+++oo    [ 28,
  //   0000_0001,  == ooooooo+ ==    1,
  //   0000_0110 ]    ooooo++o       6 ]
  // lampsMatrix is 16x8, not 8x3
  for(int i=0;i<8;i++){
    for(int j=0;j<16;j++){
      if((lampsMatrix[i] >> (15-j))%2 == 0){ // Lamp is off
        lampImage = gtk_image_new_from_file("./icons/off.png");
      } else { // Lamp is on
        lampImage = gtk_image_new_from_file("./icons/on.png");
      }
      gtk_flow_box_append(GTK_FLOW_BOX(lampsBox),lampImage);
    }
  }
}

static void runEmulation(){
  char *text;
  uint64_t registors;

  GtkTextIter start, end;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(lampanelInput));
  gtk_text_buffer_get_bounds (buffer, &start, &end);
  text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
  // ^^^ IDK what happens here I took it from stackoverflow and it somehow works
  // related to code input / gtk_text_view

  // lamp switching works like this:
  // lampsMatrix[0] = 4;
  // lampsMatrix[3] = 23;
  // or like this:
  registors = runE97(&lampsMatrix, text);

  // Update registors output
  for(int i=0;i<4;i++){
    // label = uint16_t(registors >> (16*i));
  }

  // Update lampanel
  gtk_flow_box_remove_all(GTK_FLOW_BOX(lampsBox));
  generateLamps();

  g_free(text);
}

static void windowActivate(GApplication *app){
  GtkWidget *window, *box1, *box3, *runProgram, *titlebar;
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
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_titlebar(GTK_WINDOW(window), titlebar);

  // Horizontal Main Box
  box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  //gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);
  gtk_window_set_child(GTK_WINDOW(window), box1);

  // Vertical Child Box (Right)
  box3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box3), TRUE);
  gtk_box_append(GTK_BOX(box1), box3);

  // Lamps field
  // Времени потрачено на лампы: 9 часов
  lampsBox = gtk_flow_box_new();
  gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(lampsBox), TRUE);
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(lampsBox), 16);
  gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(lampsBox), 16);
  gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(lampsBox), 0);
  gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(lampsBox), 0);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(lampsBox), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(box3), lampsBox);
  generateLamps();

  // Code Input Field
  lampanelInput = gtk_text_view_new();
  gtk_box_append(GTK_BOX(box3), lampanelInput);

  // Vertical Child Box (Left)
  regsBox = gtk_flow_box_new();
  gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(regsBox), TRUE);
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(regsBox), 1);
  gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(regsBox), 1);
  gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(regsBox), 0);
  gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(regsBox), 0);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(regsBox), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(box1), regsBox);

  // Registors
  r0 = gtk_label_new("R0: 0000 0000 0000 0000");
  r1 = gtk_label_new("R1: 0000 0000 0000 0000");
  r2 = gtk_label_new("R2: 0000 0000 0000 0000");
  r3 = gtk_label_new("R3: 0000 0000 0000 0000");
  exitStatus = gtk_label_new("Status:");
  gtk_flow_box_append(GTK_FLOW_BOX(regsBox), r0);
  gtk_flow_box_append(GTK_FLOW_BOX(regsBox), r1);
  gtk_flow_box_append(GTK_FLOW_BOX(regsBox), r2);
  gtk_flow_box_append(GTK_FLOW_BOX(regsBox), r3);
  gtk_flow_box_append(GTK_FLOW_BOX(regsBox), exitStatus);

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

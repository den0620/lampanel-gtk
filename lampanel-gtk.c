#include <gtk/gtk.h>
#include <stdint.h>
#include <stdlib.h>
//#include <gtksourceview/gtksource.h>

static GtkWidget *lampanelInput, *lampImage;
static uint16_t r0, r1, r2, r3, pc, sp, ps;
static GtkWidget *lampsBox, *regsBox, *CVNames, *CVBin, *CVHex, *CVuDec, *CVsDec;
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


static void generateValues(){
  // ~CVNames~ CVBin CVHex CVuDec CVsDec
  uint16_t values[7] = {r0,r1,r2,r3,pc,sp,ps};
  // CVBin
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVBin));
  for(int num=0;num<7;num++){
    GtkWidget *label;
    char buf[17] = {'\0'};
    for(int i=15;i>=0;i--){
      buf[15-i] = '0' + ((values[num] >> i) & 1);
    }
    label = gtk_label_new(buf);
    gtk_flow_box_append(GTK_FLOW_BOX(CVBin), label);
    //g_free(label);
  }
  // CVHex
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVHex));
  for(int num=0;num<7;num++){
    GtkWidget *label;
    char buf[5] = {'\0'};
    char table[17] = "0123456789ABCDEF";
    for(int i=3;i>=0;i--){
      buf[3-i] = table[((values[num] >> (4*i)) & 0xF)];
    }
    label = gtk_label_new(buf);
    gtk_flow_box_append(GTK_FLOW_BOX(CVHex), label);
    //g_free(label);
  }
  // CVuDec
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVuDec));
  for(int num=0;num<7;num++){
    GtkWidget *label;
    char buf[8];
    sprintf(buf, "%d", values[num]);
    label = gtk_label_new(buf);
    gtk_flow_box_append(GTK_FLOW_BOX(CVuDec), label);
    //g_free(label);
  }
  // CVsDec
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVsDec));
  for(int num=0;num<7;num++){
    GtkWidget *label;
    char buf[8];
    sprintf(buf, "%d", (int16_t)values[num]);
    label = gtk_label_new(buf);
    gtk_flow_box_append(GTK_FLOW_BOX(CVsDec), label);
    //g_free(label);
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

  // Plan:
  // Compile program entered by user
  // Make func to run line by line

  // Update registors output
  //for(int i=0;i<4;i++){
  //  // label = uint16_t(registors >> (16*i));
  //}

  // Update lampanel
  //gtk_flow_box_remove_all(GTK_FLOW_BOX(lampsBox));
  //generateLamps();
  // ^^^ needs to executed when interacting with lamps

  g_free(text);
}

static void windowActivate(GApplication *app){
  GtkWidget *window, *mainVertical, *childHorizontalU, *childHorizontalL, *compiledOutput, *mainHorizontalR, *memoryOverview, *runProgram, *titlebar;
  GdkPixbuf *icon;

  // Play Button
  runProgram = gtk_button_new_from_icon_name("media-playback-start-symbolic");
  g_signal_connect(runProgram, "clicked", G_CALLBACK(runEmulation), NULL);
  printf("play\n");

  // Titlebar [>    -^x]
  titlebar = gtk_header_bar_new();
  gtk_header_bar_pack_start(GTK_HEADER_BAR(titlebar), GTK_WIDGET(runProgram));
  printf("titlebar\n");

  // Window
  window = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(window), "Lampanel GTK");
  gtk_window_set_icon_name(GTK_WINDOW(window), "application-x-executable");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_titlebar(GTK_WINDOW(window), titlebar);
  printf("window\n");

  // Main vertical box / > upper (childHorizontalU)
  //                   \ > lower (childHorizontalL)
  mainVertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  //gtk_box_set_homogeneous(GTK_BOX(mainVertical), TRUE);
  gtk_window_set_child(GTK_WINDOW(window), mainVertical);
  printf("mainVertical\n");

  // Horizontal Child Box (Upper) (for lamps & registor overview)
  childHorizontalU = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(childHorizontalU), TRUE);
  gtk_box_append(GTK_BOX(mainVertical), childHorizontalU);
  printf("mainHorizontal\n");

  // Horizontal Child Box (Lower) (for code & compiled & memory)
  childHorizontalL = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(mainVertical), childHorizontalL);
  printf("childHorizontalL\n");

  // Lamps field
  lampsBox = gtk_flow_box_new();
  gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(lampsBox), TRUE);
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(lampsBox), 16);
  gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(lampsBox), 16);
  gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(lampsBox), 0);
  gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(lampsBox), 0);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(lampsBox), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(childHorizontalU), lampsBox);
  generateLamps();
  printf("lampsBox\n");

  // Box for registors <-------------------------------> (mainHorizontalR)
  //                      |      |     |     |      |
  //                      v      v     v     v      v 
  //                   CVNames CVbin CVHex CVuDec CVsDec
  mainHorizontalR = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  //gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(regsBox), TRUE);//gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(regsBox), 1);//gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(regsBox), 1);//gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(regsBox), 0);//gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(regsBox), 0);//gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(regsBox), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(childHorizontalU), mainHorizontalR);
  printf("mainHorizontalR(ight)\n");
  // Names
  CVNames = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(CVNames), TRUE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVNames);
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("R0"));
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("R1"));
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("R2"));
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("R3"));
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("PC"));
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("SP"));
  gtk_box_append(GTK_BOX(CVNames), gtk_label_new("PS"));
  // Binary Values
  printf("cvnames\n");
  CVBin = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVBin), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVBin), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVBin);
  printf("cvbin\n");
  // Hexadecimal Values
  CVHex = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVHex), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVHex), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVHex);
  printf("cvhex\n");
  // Unsigned Decimal Values
  CVuDec = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVuDec), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVuDec), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVuDec);
  printf("cvudec\n");
  // Signed Decimal Values
  CVsDec = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVsDec), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVsDec), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVsDec);
  printf("cvsdec\n");
  // Generate All Values
  generateValues();

  // Code Input Field
  lampanelInput = gtk_text_view_new();
  gtk_box_append(GTK_BOX(childHorizontalL), lampanelInput);
  printf("lampanelInput\n");

  // Compiled File Field
  compiledOutput = gtk_text_view_new();
  gtk_box_append(GTK_BOX(childHorizontalL), compiledOutput);
  printf("compiledOutput\n");

  // Memory Overview Field
  memoryOverview = gtk_text_view_new();
  gtk_box_append(GTK_BOX(childHorizontalL), memoryOverview);
  printf("memoryOverview\n");

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv){
  GtkApplication *window;
  int status;

  window = gtk_application_new("site.degroland.lampanel-gtk", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(window, "activate", G_CALLBACK(windowActivate), NULL);
  status = g_application_run(G_APPLICATION(window), argc, argv);
  g_object_unref(window);

  return status;
}

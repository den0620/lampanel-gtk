#include <gtk/gtk.h>
#include <stdint.h>
#include <stdlib.h>
#include <gtksourceview/gtksource.h>

static GtkWidget *lampanelInput, *lampImage;
static GtkSourceBuffer *codeBuffer;
static GtkSourceLanguageManager *codeLangMan;
static GtkSourceLanguage *codeLang;
static uint16_t r0, r1, r2, r3, pc, sp, ps;
static GtkWidget *lampsBox, *regsBox, *CVNames, *CVBin, *CVHex, *CVuDec, *CVsDec;
static uint16_t lampsMatrix[8]; // Actually not a matrix, int 0000 0000 0000 0000 x8 rows


// Functions `generateLamps` or/and `generateValues` should be ran every time there is a change to `lampsMatrix` or/and registors


static void generateLamps(){
  GtkWidget *lampImage;
  // Lamps changing by lampsMatrix like this: (8x3 example)
  // [ 0001_1100,     ooo+++oo    [ 28,
  //   0000_0001,  == ooooooo+ ==    1,
  //   0000_0110 ]    ooooo++o       6 ]
  // lampsMatrix is 16x8, not 8x3
  gtk_flow_box_remove_all(GTK_FLOW_BOX(lampsBox));
  for(int i=0;i<8;i++){
    for(int j=0;j<16;j++){
      if((lampsMatrix[i] >> (15-j))%2 == 0){
        lampImage = gtk_image_new_from_file("./icons/off.png");
      } else {
        lampImage = gtk_image_new_from_file("./icons/on.png");
      }
      gtk_flow_box_append(GTK_FLOW_BOX(lampsBox),lampImage);
    }
  }
}




static void generateValues(){
  // ~CVNames~ CVBin CVHex CVuDec CVsDec
  uint16_t values[7] = {r0,r1,r2,r3,pc,sp,ps};
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVBin));
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVHex));
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVuDec));
  gtk_flow_box_remove_all(GTK_FLOW_BOX(CVsDec));
  for(int num=0;num<7;num++){
    GtkWidget *labelB, *labelH, *labelUD, *labelSD;
    char bufB[17] = "\0", bufH[5], bufUD[8], bufSD[8];
    char table[17] = "0123456789ABCDEF";
    for(int i=15;i>=0;i--){
      bufB[15-i] = '0' + ((values[num] >> i) & 1);
      if(i%4==0){
        bufH[3-i/4] = table[((values[num] >> (i)) & 0xF)];
      }
    }
    sprintf(bufUD, "%d", values[num]);
    sprintf(bufSD, "%d", (int16_t)values[num]);
    labelB = gtk_label_new(bufB);
    labelH = gtk_label_new(bufH);
    labelUD = gtk_label_new(bufUD);
    labelSD = gtk_label_new(bufSD);
    gtk_flow_box_append(GTK_FLOW_BOX(CVBin), labelB);
    gtk_flow_box_append(GTK_FLOW_BOX(CVHex), labelH);
    gtk_flow_box_append(GTK_FLOW_BOX(CVuDec), labelUD);
    gtk_flow_box_append(GTK_FLOW_BOX(CVsDec), labelSD);
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

  // Plan:
  // Compile program entered by user
  // Make func to run line by line

  g_free(text);
}

static void windowActivate(GApplication *app){
  GtkWidget *window, *mainVertical, *childHorizontalU, *childHorizontalL, *compiledOutput, *mainHorizontalR, *memoryOverview, *runProgram, *titlebar, *scrollWindow;
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
  //gtk_window_set_icon_name(GTK_WINDOW(window), "application-x-executable");
  gtk_window_set_icon_name(GTK_WINDOW(window), "./icons/on.png");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_titlebar(GTK_WINDOW(window), titlebar);
  printf("window\n");

  // Main vertical box / > upper (childHorizontalU)
  //                   \ > lower (childHorizontalL)
  mainVertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(mainVertical), TRUE);
  gtk_window_set_child(GTK_WINDOW(window), mainVertical);
  printf("mainVertical\n");

  // Horizontal Child Box (Upper) (for lamps & registor overview)
  childHorizontalU = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(childHorizontalU), FALSE);
  gtk_box_append(GTK_BOX(mainVertical), childHorizontalU);
  printf("mainHorizontal\n");

  // Horizontal Child Box (Lower) (for code & compiled & memory)
  childHorizontalL = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(childHorizontalL), TRUE);
  gtk_box_append(GTK_BOX(mainVertical), childHorizontalL);
  printf("childHorizontalL\n");

  // Lamps field
  lampsBox = gtk_flow_box_new();
  gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(lampsBox), TRUE);
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(lampsBox), 16);
  gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(lampsBox), 16);
  gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(lampsBox), -10);
  gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(lampsBox), -10); // Set -100 if you want segfault)
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(lampsBox), GTK_SELECTION_NONE);
  gtk_widget_set_hexpand(GTK_WIDGET(lampsBox), TRUE);
  gtk_box_append(GTK_BOX(childHorizontalU), lampsBox);
  generateLamps();
  printf("lampsBox\n");

  // Box for registors <-------------------------------> (mainHorizontalR)
  //                      |      |     |     |      |
  //                      v      v     v     v      v 
  //                   CVNames CVbin CVHex CVuDec CVsDec
  mainHorizontalR = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(mainHorizontalR), FALSE);
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
  // Values
  printf("cvnames\n");
  CVBin = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVBin), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVBin), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVBin);
  printf("cvbin\n");
  CVHex = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVHex), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVHex), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVHex);
  printf("cvhex\n");
  CVuDec = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVuDec), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVuDec), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVuDec);
  printf("cvudec\n");
  CVsDec = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(CVsDec), 1);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(CVsDec), GTK_SELECTION_NONE);
  gtk_box_append(GTK_BOX(mainHorizontalR), CVsDec);
  printf("cvsdec\n");
  generateValues();


  // Code Input Field
  codeBuffer = gtk_source_buffer_new(NULL);
  codeLangMan = gtk_source_language_manager_new();
  gtk_source_language_manager_append_search_path(codeLangMan, (const gchar *)("./langs/"));
  //gtk_source_language_manager_get_language_from_file(codeLangMan, langsPath); // outdated
  codeLang = gtk_source_language_manager_get_language(codeLangMan, "asm");
  if(codeLang == NULL){
  	printf("Something wrong with codeLang (==NULL)");
  }

  gtk_text_buffer_set_enable_undo(GTK_TEXT_BUFFER(codeBuffer), TRUE);
  gtk_source_buffer_set_highlight_syntax(GTK_SOURCE_BUFFER(codeBuffer), TRUE);
  /* //gtk_source_buffer_ensure_highlight(GTK_SOURCE_BUFFER(codeBuffer), TRUE); */
  gtk_source_buffer_set_highlight_matching_brackets(GTK_SOURCE_BUFFER(codeBuffer), TRUE);
  gtk_source_buffer_set_highlight_syntax(GTK_SOURCE_BUFFER(codeBuffer), TRUE);
  gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(codeBuffer), GTK_SOURCE_LANGUAGE(codeLang));
  lampanelInput = gtk_source_view_new_with_buffer(codeBuffer);
  gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(lampanelInput), TRUE);
  // Code Input Scroll Window
  scrollWindow = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrollWindow), 100);
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrollWindow), 200);
  gtk_scrolled_window_set_kinetic_scrolling(GTK_SCROLLED_WINDOW(scrollWindow), TRUE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrollWindow), lampanelInput);
  gtk_box_append(GTK_BOX(childHorizontalL), scrollWindow);

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

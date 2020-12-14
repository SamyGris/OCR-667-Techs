#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../../OCR-667-Techs/Pretreats/median.h"
#include "../../OCR-667-Techs/Pretreats/otsu.h"
#include "../../OCR-667-Techs/Pretreats/pixel_operations.h"
#include "../../OCR-667-Techs/Pretreats/grayscale.h"
#include "../../OCR-667-Techs/Segmentation/rlsa.h"

//Make them global


GtkWidget  *window;
GtkWidget  *grid;
GtkWidget *page_fixed0;
GtkWidget *page_fixed1;
GtkWidget *page_fixed2;
GtkWidget *page_fixed3;
GtkWidget *page_fixed4;
GtkWidget *page_fixed5;

GtkWidget *button1;
GtkWidget *quit_button;
//GtkWidget *buttonS;

GtkWidget *image1;
GtkWidget *image2;
GtkWidget *image3;
GtkWidget *image4;
GtkWidget *image5;

GtkWidget *image_p0;
GtkWidget *image_p1;
GtkWidget *image_p2;
GtkWidget *image_p3;
GtkWidget *image_p4;

GtkWidget *gridimage;
GtkWidget *stack1;
GtkWidget *frame1;
GtkWidget *labelchoose;




GtkBuilder *builder;


int main(int argc, char *argv[])
{
  gtk_init(NULL,NULL);

  builder = gtk_builder_new_from_file("test_glade.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder,"window"));
  
  g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_builder_connect_signals(builder,NULL);
  
  
  button1 = GTK_WIDGET(gtk_builder_get_object(builder,"button1"));
  quit_button = GTK_WIDGET(gtk_builder_get_object(builder,"quit_button"));
  grid  = GTK_WIDGET(gtk_builder_get_object(builder,"grid"));
  page_fixed0 =  GTK_WIDGET(gtk_builder_get_object(builder,"page_fixed0"));
  page_fixed1 =  GTK_WIDGET(gtk_builder_get_object(builder,"page_fixed1"));
  page_fixed2 =  GTK_WIDGET(gtk_builder_get_object(builder,"page_fixed2"));
  page_fixed3 =  GTK_WIDGET(gtk_builder_get_object(builder,"page_fixed3"));
  page_fixed4 =  GTK_WIDGET(gtk_builder_get_object(builder,"page_fixed4"));
  page_fixed5 =  GTK_WIDGET(gtk_builder_get_object(builder,"page_fixed5"));
  gridimage  = GTK_WIDGET(gtk_builder_get_object(builder,"gridimage"));
  image_p0 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p0"));
  image_p1 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p1"));
  image_p2 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p2"));
  image_p3 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p3"));
  image_p4 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p4"));
  stack1 = GTK_WIDGET(gtk_builder_get_object(builder, "stack1"));
  frame1 = GTK_WIDGET(gtk_builder_get_object(builder, "frame1"));


  GdkColor color; //default background color
  color.red =0x8c8c ;
  color.green = 0xb3b3;
  color.blue = 0xdada;
  gtk_widget_modify_bg(GTK_WIDGET(window),GTK_STATE_NORMAL,&color);



  image1 = NULL;
  image2 = NULL;
  image3 = NULL;
  image4 = NULL;
  


  gtk_widget_show(window);
 
  
  gtk_main();


  return EXIT_SUCCESS;      
  
}

void on_quit_button_clicked(GtkButton *b1)
{
  gtk_main_quit();
}

void on_button1_clicked(GtkFileChooserButton *b)
{
  gtk_widget_hide(image_p0);
  gtk_widget_hide(image_p1);
  gtk_widget_hide(image_p2);
  gtk_widget_hide(image_p3);
  gtk_widget_hide(image_p4);
 
  printf("file name = %s\n", gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(b)) );
  printf("folder name = %s\n", gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER(b)) );

  char* filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(b));
  int i = 0;
  int res = 0;
  while (filename[i])
    {
      res ++;
      i++;
    }
  printf("%d\n",res);

  FILE *f1;

  if((filename[res-1]=='g')&&(((filename[res-2]=='p')||
  (filename[res-2]=='n'))&&((filename[res-3]=='j')||(filename[res-3]=='p'))))
    {
      if(image1)
	{
	  gtk_container_remove(GTK_CONTAINER(page_fixed0),image1); //suppression de l'image  en place
	  gtk_container_remove(GTK_CONTAINER(page_fixed1),image2);
	  gtk_container_remove(GTK_CONTAINER(page_fixed2),image3);
	  gtk_container_remove(GTK_CONTAINER(page_fixed3),image4);
	  gtk_container_remove(GTK_CONTAINER(page_fixed3),image5);
	}
      gtk_widget_hide(image_p0);
      gtk_widget_hide(image_p1);
      gtk_widget_hide(image_p2);
      gtk_widget_hide(image_p3);
      gtk_widget_hide(image_p4);
      char cmd[2048];
      int hor = 0, ver = 0;
      int j, h, v;
      
      
      //resize image
      
      sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
      
      f1 = popen(cmd, "r");
      
      strcpy(cmd,"");
      fgets(cmd, 512, f1);
      fclose (f1);
      
      h = v = 1;
      
      if (strlen(cmd)) {
	for (j=0; j<strlen(cmd)-1; j++) if (cmd[j] == 'x') break;
	if (cmd[j] == 'x') { // x is a delimiter between horizontal and vertical
	  cmd[j] = 0;
	  sscanf(cmd,"%d",&h);
	  sscanf(&cmd[j+1], "%d", &v);
	}
      }
      
      if (h < 100 || v < 100 ) {
	printf("**** questionable image: %s\n",filename);
	return;	// probably a bad image
      }
      
      //	---------------------------------------------------------------
      //	RESIZE image.  convert() is part of ImageMagick
      //	---------------------------------------------------------------
      
      int width = 488; 
      int height = 468;
      
      sprintf(cmd, "convert \"%s\" -resize %dx%d tmp.jpg", filename, width, height);
      system(cmd);
      
      strcpy(filename,"tmp.jpg");
      
      //	---------------------------------------------------------------
      //	Get new image dimensions  - actual dimensions will be in the
      //	range of the conversion. Conversion will maintain ratio.
      //	---------------------------------------------------------------
      
      sprintf(cmd, "identify -format %%wx%%h \"%s\"\n", filename);
      f1 = popen(cmd, "r");
      strcpy(cmd,"");
      fgets(cmd, 512, f1);
      fclose (f1);
      h = v = 1;
      
      if (strlen(cmd)) {
	for (j=0; j<strlen(cmd)-1; j++) if (cmd[j] == 'x') break;
	if (cmd[j] == 'x') {
	  cmd[j] = 0;
	  sscanf(cmd,"%d",&h);
	  sscanf(&cmd[j+1], "%d", &v);
	}
      }
      
      //	---------------------------------------------------------------
      //	h and v are now the actual new dimensions which you can use
      //	to better center your image in the GUI - not used here
      //	---------------------------------------------------------------


      system("mkdir cache");
      SDL_Surface *img = IMG_Load(filename);
      
      SDL_Surface *grayscale_img = grayscale(img);
      SDL_Surface *median_img = noise_canceled(grayscale_img);
      SDL_Surface *binarized_img = black_n_white(median_img);
      int** rlsaor_mat = rlsa_or(binarized_img);
      SDL_Surface *rlsaor_img;
      set_image(rlsaor_mat, rlsaor_img);

      SDL_SaveBMP(grayscale_img, "cache/grayscale.png");
      SDL_SaveBMP(median_img, "cache/median.png");
      SDL_SaveBMP(binarized_img, "cache/binarized.png");
      SDL_SaveBMP(rlsaor_img, "cache/rlsa.png");

      image1 = gtk_image_new_from_file (filename);
      image2 = gtk_image_new_from_file ("cache/grayscale.png");
      image3 = gtk_image_new_from_file ( "cache/median.png");
      image4 = gtk_image_new_from_file ("cache/binarized.png");
      image5 = gtk_image_new_from_file ("cache/rlsa.png");
      
      gtk_container_add (GTK_CONTAINER (page_fixed0), image1);
      gtk_container_add (GTK_CONTAINER (page_fixed1), image2);
      gtk_container_add (GTK_CONTAINER (page_fixed2), image3);
      gtk_container_add (GTK_CONTAINER (page_fixed3), image4);
      gtk_container_add (GTK_CONTAINER (page_fixed4), image5);
      
      gtk_widget_show(image1);
      gtk_widget_show(image2);
      gtk_widget_show(image3);
      gtk_widget_show(image4);
      gtk_widget_show(image5);
      
      gtk_fixed_move (GTK_FIXED(page_fixed0), image1, hor, ver);
      gtk_fixed_move (GTK_FIXED(page_fixed1), image2, hor, ver);
      gtk_fixed_move (GTK_FIXED(page_fixed2), image3, hor, ver);
      gtk_fixed_move (GTK_FIXED(page_fixed3), image4, hor, ver);
      gtk_fixed_move (GTK_FIXED(page_fixed4), image5, hor, ver);
      
      system("rm tmp.jpg");
      system("rm -rf cache");
      
    }
  else
    {
      // gtk_label_set_text(GTK_LABEL(labelerror),(const gchar*) "Not a image");
      //printf("c'est pas une image %s", "...");
      
    }
  
}


  


  


			 



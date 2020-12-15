#include <sys/types.h>
#include <sys/stat.h>
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



GtkWidget  *window; //we set global variables
GtkWidget  *grid;
GtkWidget *page_fixed0;
GtkWidget *page_fixed1;
GtkWidget *page_fixed2;
GtkWidget *page_fixed3;
GtkWidget *page_fixed4;
GtkWidget *page_fixed5;

GtkWidget *button1;
GtkWidget *quit_button;

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

char* filename;

GtkBuilder *builder;


int main() //our main loop
{
  gtk_init(NULL,NULL); //we initialize the interface

  builder = gtk_builder_new_from_file("gui.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder,"window")); //we pick up all the GtkFixed
  
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
  
  image_p0 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p0")); 
  image_p1 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p1"));
  image_p2 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p2"));
  image_p3 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p3"));
  image_p4 = GTK_WIDGET(gtk_builder_get_object(builder, "image_p4"));


  GdkColor color; //we change background color
  color.red = color.green = color.blue = 0xe0e0;
  gtk_widget_modify_bg(GTK_WIDGET(window),GTK_STATE_NORMAL,&color);



  image1 = NULL; //we initialize our images
  image2 = NULL;
  image3 = NULL;
  image4 = NULL;
  image5 = NULL;
  


  gtk_widget_show(window);
 
  
  
  gtk_main();

  return EXIT_SUCCESS;      
  
}

void on_quit_button_clicked()
{
  gtk_main_quit();
}

void on_button1_clicked(GtkFileChooserButton *b)
{
   filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(b));
}

void on_execute_button_clicked()
{
  gtk_widget_hide(image_p0);
  gtk_widget_hide(image_p1);
  gtk_widget_hide(image_p2);
  gtk_widget_hide(image_p3);
  gtk_widget_hide(image_p4);
  
  
  
  int i = 0;
  int res = 0;
  while (filename[i])//filename length calculation
    {
      res ++;
      i++;
    }
  
  if((filename[res-1]=='g')&&(((filename[res-2]=='p')
			       ||(filename[res-2]=='n'))&&((filename[res-3]=='j')||(filename[res-3]=='p'))))
    {
      if(image1)
	{
	  gtk_container_remove(GTK_CONTAINER(page_fixed0),image1); //we erase the previous pics (if previous pics are present)
	  gtk_container_remove(GTK_CONTAINER(page_fixed1),image2);
	  gtk_container_remove(GTK_CONTAINER(page_fixed2),image3);
	  gtk_container_remove(GTK_CONTAINER(page_fixed3),image4);
	  gtk_container_remove(GTK_CONTAINER(page_fixed4),image5);
	}
      gtk_widget_hide(image_p0);
      gtk_widget_hide(image_p1);
      gtk_widget_hide(image_p2);
      gtk_widget_hide(image_p3);
      gtk_widget_hide(image_p4);
      
    }

  //we connect the graphic interface with all the OCR functions.
  
  mkdir("cache", 0777); //we create the folder that will contain every step picture
  
  SDL_Surface *img = IMG_Load(filename); //we convert the jpg/png/bmp file to SDL_Surface
  gtk_container_remove(GTK_CONTAINER(page_fixed0),image1); //we remove the image1 from the container if present
  SDL_SaveBMP(img, "cache/base"); //we save the sdl_surface as a BMP (work as a jpg/png in Glade) in the step folder
  image1 = gtk_image_new_from_file("cache/base"); //we load the bmp file as a gtk image widget
  gtk_container_add (GTK_CONTAINER (page_fixed0), image1); //we put the widget his respective container
  gtk_widget_show(image1); //we show the imqge
  
  
  SDL_Surface *grayscale_img = grayscale(img);
  gtk_container_remove(GTK_CONTAINER(page_fixed1),image2);
  SDL_SaveBMP(grayscale_img, "cache/grayscale");
  image2 = gtk_image_new_from_file("cache/grayscale");
  gtk_container_add (GTK_CONTAINER(page_fixed1), image2);
  gtk_widget_show(image2);
  
  SDL_Surface *median_img = noise_canceled(grayscale_img);
  gtk_container_remove(GTK_CONTAINER(page_fixed2),image3);
  SDL_SaveBMP(median_img, "cache/median");
  image3 = gtk_image_new_from_file( "cache/median");
  gtk_container_add (GTK_CONTAINER(page_fixed2), image3);
  gtk_widget_show(image3);
  
  SDL_Surface *binarized_img = black_n_white(grayscale_img);
  gtk_container_remove(GTK_CONTAINER(page_fixed3),image4);
  SDL_SaveBMP(binarized_img, "cache/binarized");
  image4 = gtk_image_new_from_file("cache/binarized");
  gtk_container_add (GTK_CONTAINER(page_fixed3), image4);
  gtk_widget_show(image4);
  
  int** rlsaor_mat = rlsa_or(binarized_img);
  SDL_Surface *rlsaor_img = binarized_img;
  set_image(rlsaor_mat, rlsaor_img);
  gtk_container_remove(GTK_CONTAINER(page_fixed4),image5);
  SDL_SaveBMP(rlsaor_img, "cache/rlsa");
  image5 = gtk_image_new_from_file("cache/rlsa");
  gtk_container_add (GTK_CONTAINER(page_fixed4), image5);
  gtk_widget_show(image5);

}


  


  


			 



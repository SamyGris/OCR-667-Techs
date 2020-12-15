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

char* filename;




GtkBuilder *builder;


int main()
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
  //gtk_widget_modify_bg(GTK_WIDGET(window),GTK_STATE_NORMAL,&color);



  image1 = NULL;
  image2 = NULL;
  image3 = NULL;
  image4 = NULL;
  


  gtk_widget_show(window);
 
  
  
  gtk_main();
  unlink("cache");
  rmdir("cache");

  return EXIT_SUCCESS;      
  
}

void on_quit_button_clicked(GtkButton *b1)
{
  gtk_main_quit();
}

void on_button1_clicked(GtkFileChooserButton *b)
{
   filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(b));
   printf("%s \n", filename);
}

void on_execute_button_clicked(GtkFileChooserButton *b2)
{
  printf("%s \n", filename);
  unlink("cache");
  rmdir("cache");
  gtk_widget_hide(image_p0);
  gtk_widget_hide(image_p1);
  gtk_widget_hide(image_p2);
  gtk_widget_hide(image_p3);
  gtk_widget_hide(image_p4);


 
  int i = 0;
  int res = 0;
  while (filename[i])
    {
      res ++;
      i++;
    }
  printf("%d\n",res);

  if((filename[res-1]=='g')&&(((filename[res-2]=='p')
||(filename[res-2]=='n'))&&((filename[res-3]=='j')||(filename[res-3]=='p'))))
    {
      if(image1)
	{
	  gtk_container_remove(GTK_CONTAINER(page_fixed0),image1); //suppression de l'image  en place
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
      
      //	---------------------------------------------------------------
      //	h and v are now the actual new dimensions which you can use
      //	to better center your image in the GUI - not used here
      //	---------------------------------------------------------------


      mkdir("cache", 0777);

      SDL_Surface *img = IMG_Load(filename);
      remove("cache/base");
      gtk_container_remove(GTK_CONTAINER(page_fixed0),image1);
      SDL_SaveBMP(img, "cache/base");
      image1 = gtk_image_new_from_file("cache/base");
      gtk_container_add (GTK_CONTAINER (page_fixed0), image1);
      gtk_widget_show(image1);
     
      
      SDL_Surface *grayscale_img = grayscale(img);
      remove("cache/grayscale");
      gtk_container_remove(GTK_CONTAINER(page_fixed1),image2);
      SDL_SaveBMP(grayscale_img, "cache/grayscale");
      image2 = gtk_image_new_from_file("cache/grayscale");
      gtk_container_add (GTK_CONTAINER(page_fixed1), image2);
      gtk_widget_show(image2);
      
      SDL_Surface *median_img = noise_canceled(grayscale_img);
      remove("cache/median");
      gtk_container_remove(GTK_CONTAINER(page_fixed2),image3);
      SDL_SaveBMP(median_img, "cache/median");
      image3 = gtk_image_new_from_file( "cache/median");
      gtk_container_add (GTK_CONTAINER(page_fixed2), image3);
      gtk_widget_show(image3);
      
      SDL_Surface *binarized_img = black_n_white(grayscale_img);
      remove("cache/binarized");
      gtk_container_remove(GTK_CONTAINER(page_fixed3),image4);
      SDL_SaveBMP(binarized_img, "cache/binarized");
      image4 = gtk_image_new_from_file("cache/binarized");
      gtk_container_add (GTK_CONTAINER(page_fixed3), image4);
      gtk_widget_show(image4);
      
      int** rlsaor_mat = rlsa_or(binarized_img);

      SDL_Surface *rlsaor_img = binarized_img;
      set_image(rlsaor_mat, rlsaor_img);
      remove("cache/rlsa");
      gtk_container_remove(GTK_CONTAINER(page_fixed4),image5);
      SDL_SaveBMP(rlsaor_img, "cache/rlsa");
      image5 = gtk_image_new_from_file("cache/rlsa");
      gtk_container_add (GTK_CONTAINER(page_fixed4), image5);
      gtk_widget_show(image5);

      /* gtk_container_remove(GTK_CONTAINER(page_fixed0),image1);
      gtk_container_remove(GTK_CONTAINER(page_fixed1),image2);
      gtk_container_remove(GTK_CONTAINER(page_fixed2),image3);
      gtk_container_remove(GTK_CONTAINER(page_fixed3),image4);
      gtk_container_remove(GTK_CONTAINER(page_fixed4),image5);*/
}


  


  


			 



#include "qwirkle-base.h"

static void activate( GtkApplication *app, gpointer userData )
{
  GtkCssProvider *css = gtk_css_provider_new();
  if( !gtk_css_provider_load_from_data( GTK_CSS_PROVIDER( css ), CSS_BUTTON_STYLE, -1, NULL) ) {
    g_print("Error loading css" );
    gtk_main_quit();
  }
  
  // style buttons
  gtk_style_context_add_provider_for_screen( gdk_screen_get_default(), GTK_STYLE_PROVIDER( css ), GTK_STYLE_PROVIDER_PRIORITY_USER );
  
  // Set window size to 300x300
  gtk_window_set_default_size( GTK_WINDOW( window ), WINDOW_WIDTH, WINDOW_HEIGHT );
  
  // Set window title to 'Qwirkle!'
  gtk_window_set_title( GTK_WINDOW( window ), "Qwirkle! :)" );
  
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  
  gtk_widget_show_all( window );
  
  gtk_main();
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  gtk_init( &argc, &argv );
  g_test_init( &argc, &argv, NULL );
  init();
  
  app = gtk_application_new( "nl.kdev.qwirkle", G_APPLICATION_NON_UNIQUE );
  g_signal_connect( app, "activate", G_CALLBACK( activate ), NULL );
  status = g_application_run( G_APPLICATION( app ), argc, argv );
  g_object_unref( app );

  return status;
}

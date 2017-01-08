#include "qwirkle-base.h"
#include <ctype.h>
void initTiles() 
{
  tiles = calloc( NO_TILES, sizeof( tile ) );
  
  int i, j, k = 0, t = 0;
  for( i = 0; i < NO_EQUAL; i++ )
  {
    
    for( j = 0; j < NO_SHAPES; j++ )
    {
      for( k = 0; k < NO_COLORS; k++ )
      {
        strcpy( tiles[t].shape, shapes[j] );
        strcpy( tiles[t].color, colors[k] );
        t++;
      }
    }
  }
}

void initStock()
{
  struct timespec tm;
  clock_gettime( CLOCK_REALTIME, &tm );
  srandom(tm.tv_sec + tm.tv_nsec * 1000000ul);
  int i = 0, x;
  tile r;
  stock = calloc( NO_STOCK, sizeof( tile ) );
  while(i < NO_STOCK){
    x = random() % NO_TILES+1;
    r = tiles[x];

    stock[i++] = r;
    unsetTile( x );
  }
}

void unsetTile( int pos )
{
  if( pos < NO_TILES ) {
    int i;
    for( i = pos - 1; i < NO_TILES - 1; i++ )
    {
      tiles[i] = tiles[i + 1];
    }
  }
}

void moveDown( GtkWidget *widget, gpointer userData ) {
  gint tx, ty;
  if( debug ) {
    g_print( "MoveDown now!\n" );
  }
  
  gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( widget ), "x", &tx, "y", &ty, NULL );

  gtk_layout_move( GTK_LAYOUT( layout ), GTK_WIDGET( widget ), tx, ty + TILESIZE );
}

void moveRight( GtkWidget *widget, gpointer userData ) {
  gint tx, ty;
  if( debug ) {
    g_print( "MoveLeft now!\n" );
  }
  
  gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( widget ), "x", &tx, "y", &ty, NULL );

  gtk_layout_move( GTK_LAYOUT( layout ), GTK_WIDGET( widget ), tx + TILESIZE, ty );
}

GtkWidget *getWidgetByYPos( int y, int x )
{
  xx = x;
  widgetOnYPosFound = FALSE;
  
  if( !widgetOnYPosFound && debug ) {
    g_print( "getWidgetByYPos: nothing yet on x,y: %d, %d\n", x, y);
  }
  if( debug ) {
    g_print("widgetOnYPosFound = %s\n", widgetOnYPosFound == FALSE ? "false" : "true"); 
  }
  
  gtk_container_foreach( GTK_CONTAINER( layout ), &isWidgetOnYPos, &y );
}

GtkWidget *getWidgetByXPos( int x, int y )
{
  yy = y;
  widgetOnXPosFound = FALSE;
  
  if( !widgetOnXPosFound && debug ) {
    g_print( "getWidgetByXPos: nothing yet on y,x: %d, %d\n", y, x);
  }
  
  if( debug ) {
    g_print("widgetOnXPosFound = %s\n", widgetOnXPosFound == FALSE ? "false" : "true"); 
  }
  gtk_container_foreach( GTK_CONTAINER( layout ), &isWidgetOnXPos, &x );
}

void isWidgetOnYPos( GtkWidget *widget, gpointer userdata )
{
  int widgetY, widgetX;
  int *y = (int *) userdata;
  
  gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( widget ), "y", &widgetY,  "x", &widgetX, NULL );
  if( !widgetOnYPosFound && debug ) {
    g_print( "nothing yet on y\n");
  }
  
  if( widgetX == xx && widgetY == *y ) {
    widgetOnYPos = widget;
    widgetOnYPosFound = TRUE;
  }
}

void isWidgetOnXPos( GtkWidget *widget, gpointer userdata )
{
  int widgetX, widgetY;
  int *x = (int *) userdata;
  
  gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( widget ), "x", &widgetX, "y", &widgetY, NULL );
  if( !widgetOnYPosFound && debug ) {
    g_print( "nothing yet on x\n");
  }
  
  if( widgetY == yy && widgetX == *x ) {
    widgetOnXPos = widget;
    widgetOnXPosFound = TRUE;
  }
}

void init()
{
  gboolean debug;
  node *root;
  
  debug = FALSE;
  root = (node *) malloc( sizeof( node ) );
  
  int i;
  strcpy( shapes[0], "fourpoint" );
  strcpy( shapes[1], "circle" );
  strcpy( shapes[2], "clover" );
  strcpy( shapes[3], "diamond" );
  strcpy( shapes[4], "rectangle" );
  strcpy( shapes[5], "star" );
  
  strcpy( colors[0], "red" );
  strcpy( colors[1], "green" );
  strcpy( colors[2], "blue" );
  strcpy( colors[3], "orange" );
  strcpy( colors[4], "purple" );
  strcpy( colors[5], "yellow" );
  
  int width;
  
  g_test_set_nonfatal_assertions();
  GtkWidget *vBox = gtk_layout_new( NULL, NULL );
  
  GtkAdjustment *vPosition, *hPosition;
  
  scroll = gtk_scrolled_window_new( NULL, NULL);

  initTiles();
  initStock();

  north = gtk_frame_new( NULL );
  
  GtkWidget *button;
  
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  
  stockBar = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 );
  
  // Create layout
  layout = gtk_layout_new( NULL, NULL );
  gtk_layout_set_size( GTK_LAYOUT( layout ), WINDOW_WIDTH, WINDOW_HEIGHT );
  
  // add layout to window
  gtk_layout_put( GTK_LAYOUT( vBox ), GTK_WIDGET( stockBar ), 0, 0 );

  gtk_container_add( GTK_CONTAINER( scroll ), GTK_WIDGET( layout ) );
  gtk_layout_put( GTK_LAYOUT( vBox ), GTK_WIDGET( scroll ), 0, TILESIZE );
  
  gtk_container_add( GTK_CONTAINER( window ), GTK_WIDGET( vBox ) );
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scroll ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
  
  hPosition = gtk_scrolled_window_get_hadjustment( GTK_SCROLLED_WINDOW( scroll ) );
  gtk_adjustment_set_value( GTK_ADJUSTMENT( hPosition ), WINDOW_HEIGHT / 2 );
  gtk_scrolled_window_set_hadjustment( GTK_SCROLLED_WINDOW( scroll ), GTK_ADJUSTMENT( hPosition ) );
  
  vPosition = gtk_scrolled_window_get_vadjustment( GTK_SCROLLED_WINDOW( scroll ) );
  gtk_adjustment_set_value( GTK_ADJUSTMENT( vPosition ), WINDOW_WIDTH / 2 );
  gtk_scrolled_window_set_vadjustment( GTK_SCROLLED_WINDOW( scroll ), GTK_ADJUSTMENT( vPosition ) );
  
  gtk_widget_set_size_request( GTK_WIDGET( scroll ), WINDOW_WIDTH, WINDOW_HEIGHT );
  for( i = 0; i < NO_STOCK; i++ )
  {
    // create button and add it to layout
    button = GTK_WIDGET( createButton( stock + i ) );
    gtk_box_pack_start( GTK_BOX( stockBar ), GTK_WIDGET( button ), FALSE, FALSE, 0 );
  }
  
  gtk_widget_set_size_request( GTK_WIDGET( north ), TILESIZE, TILESIZE );
  gtk_drag_dest_set( GTK_WIDGET( north ), GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT, targetList, nTargets, GDK_ACTION_COPY );
  g_signal_connect( north, "drag-data-received", G_CALLBACK( dragDataReceived ), NULL );
  g_signal_connect( north, "drag-drop", G_CALLBACK( dragDrop ), NULL );
  
  gtk_layout_put( GTK_LAYOUT( layout ), GTK_WIDGET( north ), 0, 0 );
}

GtkWidget *createImage( char *filename, char *colorFile )
{
  GdkPixbuf *temp;
  GtkWidget *image;
  cairo_status_t status0;
  cairo_status_t status1;
  
  cairo_surface_t *surface0 = cairo_image_surface_create_from_png( colorFile );
  status0 = cairo_surface_status(surface0);
  if( status0 != CAIRO_STATUS_SUCCESS )
    g_print( "Error in surface0! %s\n", cairo_status_to_string( status0 ) );
  
  cairo_surface_t *surface1 = cairo_image_surface_create_from_png( filename );
  status1 = cairo_surface_status(surface1);
  if( status1 != CAIRO_STATUS_SUCCESS )
    g_print( "Error in surface1! %s\n", cairo_status_to_string( status1 ) );

  cairo_surface_t *img = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, TILESIZE, TILESIZE );
  
  cairo_t *cr = cairo_create( img );
  
  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
  cairo_paint(cr);
  
  cairo_set_source_surface(cr, surface0, 0, 0);
  cairo_paint(cr);

  cairo_set_source_surface(cr, surface1, 0, 0);
  cairo_paint(cr);
  
  temp = gdk_pixbuf_get_from_surface( img, 0, 0, TILESIZE, TILESIZE );
  image = GTK_WIDGET( gtk_image_new_from_pixbuf( temp ) );
  
  // clean up
  cairo_destroy( cr );
  cairo_surface_destroy( img );
  cairo_surface_destroy( surface0 );
  cairo_surface_destroy( surface1 );
  g_object_unref( temp );
  
  return GTK_WIDGET( image );
}

GtkButton *createButton( tile *Tile )
{
  GtkButton *button = GTK_BUTTON(gtk_button_new());
  GtkWidget *img;
  char *resource = (char *)malloc( 30 );
  char *tmpShape = (char *)malloc( 10 );
  char *tmpResource = (char*)malloc( 20 );
  
  char *colorFile = (char *)malloc( 30 );
  char *tmpColor  = (char *)malloc( 10 );
  char *tmpColorResource  = (char *)malloc( 20 );
  
  strcpy( tmpResource, RESOURCES_PATH );
  strcpy( tmpShape, Tile->shape );
  strcat( tmpShape, IMAGE_EXTENSION );
  strcpy( resource, tmpResource );
  strcat( resource, tmpShape );
  
  strcpy( tmpColorResource, COLORS_PATH );
  strcpy( tmpColor, Tile->color );
  strcat( tmpColor, IMAGE_EXTENSION );
  strcpy( colorFile, tmpColorResource );
  strcat( colorFile, tmpColor );
  
  img = GTK_WIDGET( createImage( resource, colorFile ) );
  gtk_button_set_image( GTK_BUTTON( button ), img );
  
  gtk_drag_source_set( GTK_WIDGET( button ), GDK_BUTTON1_MASK, targetList, nTargets, GDK_ACTION_COPY );
  g_signal_connect( button, "drag-begin", G_CALLBACK( dragBegin ), NULL );
  g_signal_connect( button, "drag-data-get", G_CALLBACK( dragDataGet ), NULL );
  g_signal_connect( button, "drag-data-delete", G_CALLBACK (dragDataDelete), NULL );
  
  // lets clean up a bit
  free( tmpShape );
  free( tmpResource );
  free( tmpColorResource );
  free( tmpColor );
  return button; 
}

void deleteDestOnY( int y )
{
  getWidgetByYPos( y, TILESIZE );

  if( widgetOnYPos ) {
    
    if( strcmp( "GtkFrame", gtk_widget_get_name(widgetOnYPos) ) ) {
      if( isWidgetDragDest( GTK_WIDGET( widgetOnYPos ) ) ) {
        if( debug ) {
          g_print( "widget found! deleting!\n" );
        }
        gtk_widget_destroy( GTK_WIDGET( widgetOnYPos ) );
        return;
      }
    }
  }

  if( debug ) {
    g_print( "widget not found!\n" );
  }
}

gboolean isWidgetDragDest( GtkWidget *widget )
{
  gpointer *site;
  if( debug ) {
    g_print( "isWidgetDragDest?" );
  }
  
  if( widget ) {
    site = g_object_get_data (G_OBJECT( widget ), "gtk-drag-dest");
      
    if( site ) {
      return TRUE;
    }
  }
  
  return FALSE;
}

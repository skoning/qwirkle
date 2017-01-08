#include "qwirkle-base.h"

void dragBegin( GtkWidget *widget, GdkDragContext *context, gpointer userData ) 
{
  GdkPixbuf *pixbuf;
  GtkWidget *image;
  
  image = gtk_button_get_image( GTK_BUTTON( widget ) );
  if( gtk_image_get_storage_type( GTK_IMAGE( image ) ) == GTK_IMAGE_PIXBUF ) {
    pixbuf = gtk_image_get_pixbuf( GTK_IMAGE( image ) );
    if( pixbuf != NULL ) {
      gtk_drag_set_icon_pixbuf( context, pixbuf, 48, 48 );
    }
  }
}

void dragDataGet( GtkWidget *widget, 
                  GdkDragContext *context, 
                  GtkSelectionData *selectionData, 
                  guint targetType, 
                  guint time, 
                  gpointer userData )
{
  draggedWidget = widget;
  g_print( "Got %s\n", ( ( tile * )userData )->shape );
  
  gtk_selection_data_set( selectionData, gtk_selection_data_get_target( selectionData ), 
    sizeof( tile ) * 8, (guchar *)userData, sizeof( tile ) );
}

gboolean dragDrop( GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer userData )
{
  GdkAtom targetType;
  
  
  if (gdk_drag_context_list_targets (context))
  {
    targetType = GDK_POINTER_TO_ATOM(g_list_nth_data (gdk_drag_context_list_targets(context), TARGET_TILE));
    gtk_drag_get_data( widget, context, targetType, time );
  }
  return TRUE;
}

void dragDataDelete( GtkWidget *widget, GdkDragContext *context, GtkSelectionData *selectionData, guint targetType, guint time, gpointer userData )
{
  g_print( "destroying %s\n", gtk_widget_get_name( GTK_WIDGET( widget ) ) );
  gtk_widget_destroy( GTK_WIDGET( widget ) );
}

void dragDataReceived( GtkWidget *widget, 
                       GdkDragContext *context, 
                       gint x, 
                       gint y, 
                       GtkSelectionData *selectionData, 
                       guint targetType, 
                       guint time, 
                       gpointer userData )
{
  gboolean debug;
  node *root;
  
  gint width, height;
  gint tx, ty;

  // Get coordinates to put tile on
  gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( widget ), "x", &tx, "y", &ty, NULL );
  
  
  /*if( root->value != NULL ) {
    // g_print( "%s\n", root->value->shape );
  }*/
    
  if( debug ) g_print( "childx %d | childy %d\n", tx, ty);
  g_assert( gtk_widget_get_parent(widget) == GTK_WIDGET( layout ) );
  
  gtk_container_remove( GTK_CONTAINER( stockBar ), GTK_WIDGET( draggedWidget ) );
  gtk_layout_put( GTK_LAYOUT( layout ), GTK_WIDGET( draggedWidget ), tx, ty );
  
  gtk_drag_source_unset( GTK_WIDGET( draggedWidget ) );
  gtk_drag_dest_unset( GTK_WIDGET( widget ) );
  
  gtk_widget_destroy( GTK_WIDGET( widget ) );
  
  gtk_layout_get_size( GTK_LAYOUT( layout ), &width, &height );
  
  // create new dests
  
  // north
  getWidgetByYPos( ty - TILESIZE, tx );
  if( !widgetOnYPosFound ) {
    if( debug ) {
      g_print( "north y nothing found!\n" );
    }
    if( ty == 0 ) {
      gtk_container_foreach( GTK_CONTAINER( layout ), moveDown, NULL );
    }
    gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( draggedWidget ), "x", &tx, "y", &ty, NULL );
    if( debug ) {
      g_print( "north newchildx %d | newchildy %d\n", tx, ty);
    }
    north = gtk_frame_new( NULL );
    gtk_layout_put( GTK_LAYOUT( layout ), GTK_WIDGET( north ), tx, ty - TILESIZE );
    gtk_widget_set_size_request( GTK_WIDGET( north ), TILESIZE, TILESIZE );
    gtk_drag_dest_set( GTK_WIDGET( north ), GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT, targetList, nTargets, GDK_ACTION_COPY );
    g_signal_connect( north, "drag-data-received", G_CALLBACK( dragDataReceived ), NULL );
    g_signal_connect( north, "drag-drop", G_CALLBACK( dragDrop ), NULL );
    gtk_layout_set_size( GTK_LAYOUT( layout ), width, height + TILESIZE );
  }
  
  // east
  getWidgetByXPos( tx + TILESIZE, ty );
  if( !widgetOnXPosFound ) {
    if( debug ) {
      g_print( "east x nothing found!\n" );
    }
    gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( draggedWidget ), "x", &tx, "y", &ty, NULL );
    if( debug ) {
      g_print( "east newchildx %d | newchildy %d\n", tx, ty);
    }
    east = gtk_frame_new( NULL );
    gtk_layout_put( GTK_LAYOUT( layout ), GTK_WIDGET( east ), tx + TILESIZE, ty );
    gtk_widget_set_size_request( GTK_WIDGET( east ), TILESIZE, TILESIZE );
    gtk_drag_dest_set( GTK_WIDGET( east ), GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT, targetList, nTargets, GDK_ACTION_COPY );
    g_signal_connect( east, "drag-data-received", G_CALLBACK( dragDataReceived ), NULL );
    g_signal_connect( east, "drag-drop", G_CALLBACK( dragDrop ), NULL );
    gtk_layout_set_size( GTK_LAYOUT( layout ), width + TILESIZE, height );
  }
  
  // south
  getWidgetByYPos( ty + TILESIZE, tx );
  if( !widgetOnYPosFound ) {
    if( debug ) {
      g_print( "south y nothing found!\n" );
    }
    gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( draggedWidget ), "x", &tx, "y", &ty, NULL );
    if( debug ) {
      g_print( "south newchildx %d | newchildy %d\n", tx, ty);
    }
    south = gtk_frame_new( NULL );
    gtk_layout_put( GTK_LAYOUT( layout ), GTK_WIDGET( south ), tx, ty + TILESIZE );
    gtk_widget_set_size_request( GTK_WIDGET( south ), TILESIZE, TILESIZE );
    gtk_drag_dest_set( GTK_WIDGET( south ), GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT, targetList, nTargets, GDK_ACTION_COPY );
    g_signal_connect( south, "drag-data-received", G_CALLBACK( dragDataReceived ), NULL );
    g_signal_connect( south, "drag-drop", G_CALLBACK( dragDrop ), NULL );
    gtk_layout_set_size( GTK_LAYOUT( layout ), width, height + TILESIZE );
  }
  
  // west
  getWidgetByXPos( tx - TILESIZE, ty );
  if( !widgetOnXPosFound ) {
    if( debug ) {
      g_print( "west x nothing found!\n" );
    }
    if( tx == 0 ) {
      gtk_container_foreach( GTK_CONTAINER( layout ), moveRight, NULL );
    }
    gtk_container_child_get( GTK_CONTAINER( layout ), GTK_WIDGET( draggedWidget ), "x", &tx, "y", &ty, NULL );
    if( debug ) {
      g_print( "west newchildx %d | newchildy %d\n", tx, ty);
    }
    west = gtk_frame_new( NULL );
    gtk_layout_put( GTK_LAYOUT( layout ), GTK_WIDGET( west ), tx - TILESIZE, ty );
    gtk_widget_set_size_request( GTK_WIDGET( west ), TILESIZE, TILESIZE );
    gtk_drag_dest_set( GTK_WIDGET( west ), GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT, targetList, nTargets, GDK_ACTION_COPY );
    g_signal_connect( west, "drag-data-received", G_CALLBACK( dragDataReceived ), NULL );
    g_signal_connect( west, "drag-drop", G_CALLBACK( dragDrop ), NULL );
    gtk_layout_set_size( GTK_LAYOUT( layout ), width + TILESIZE, height );
  }
  
  deleteDestOnY( 0 );
  gtk_widget_show_all( GTK_WIDGET( layout ) );
  gtk_drag_finish (context, TRUE, FALSE, time);
}

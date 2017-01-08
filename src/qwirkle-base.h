#ifndef __QWIRKLE_H
#define __QWIRKLE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>

enum {
  TARGET_TILE
};

enum {
  DEST_NORTH,
  DEST_EAST,
  DEST_SOUTH,
  DEST_WEST
};

static GtkTargetEntry targetList[] = {
  { "TILE", GTK_TARGET_SAME_APP, TARGET_TILE }
};

static guint nTargets = G_N_ELEMENTS( targetList );

#define SHAPE_SIZE 10
#define COLOR_SIZE 7
#define NO_STOCK  6
#define NO_COLORS 6
#define NO_SHAPES 6
#define NO_EQUAL  3
#define NO_TILES NO_COLORS * NO_SHAPES * NO_EQUAL

// path to resources
#define RESOURCES_PATH "../resources/"
// path to color images
#define COLORS_PATH RESOURCES_PATH"colors/"
// tilesize
#define TILESIZE 48
// Windowsize
#define WINDOW_WIDTH  300
#define WINDOW_HEIGHT 300
// image extension
#define IMAGE_EXTENSION ".png"

#define CSS_BUTTON_STYLE ".image-button { padding: 0px; border: 1px solid #0e0e1e; }"

typedef struct {
  char color[COLOR_SIZE];
  char shape[SHAPE_SIZE];
} tile;

typedef struct NODE node;
typedef struct NODE {
  node *n;
  node *e;
  node *z;
  node *w;
  tile *value;
} node;


// all shapes
char shapes[NO_SHAPES][SHAPE_SIZE];
char colors[NO_COLORS][COLOR_SIZE];

tile *tiles;
tile *stock;

// Main window
GtkWidget *window;

// 
GtkWidget *scroll;

// Dropareas
GtkWidget *north;
GtkWidget *east;
GtkWidget *south;
GtkWidget *west;

// Fixed
GtkWidget *layout;
GtkWidget *stockBar;

// Dragged Widget
GtkWidget *draggedWidget;

/**
 * Create Tile Image
 * @param filename string Path to the image, preferable a png
 */
GtkWidget *createImage( char* filename, char* color );

/**
 * Create Tile Button
 * @param filename string Path to the image, preferable a png
 */
GtkButton *createButton( tile* Tile );

/**
 * @param GtkWidget *widget Widget being draged
 * @param GdkDragContext *context No Idea :)
 * @param gpointer userData data provided
 */
void dragBegin( GtkWidget *widget, GdkDragContext *context, gpointer userData );

/**
 * @param GtkWidget The widget being draged
 * @param GdkDragContext *context No Idea :)
 * @param GtkSelectionData *selectionData data being transfered
 * @param guint targetType Target Type
 * @param guint time Timestamp
 * @param gpointer userData Data being transfered
 */
void dragDataGet( GtkWidget *widget, 
                  GdkDragContext *context, 
                  GtkSelectionData *selectionData, 
                  guint targetType, 
                  guint time, 
                  gpointer userData );
                         
/**
 * @param GtkWidget *widget The widget being draged
 * @param GdkDragContext *context No Idea :)
 * @param gint x coordinate within the destination widget
 * @param gint y coordinate within the destination widget
 * @param guint time TimeStamp
 * @param gpointer userData Could be anything, probably pointer to tile
 */
gboolean dragDrop( GtkWidget *widget, 
                   GdkDragContext *context, 
                   gint x, 
                   gint y, 
                   guint time, 
                   gpointer userData);

/**
 * @param GtkWidget The widget being draged
 * @param GdkDragContext *context No Idea :)
 * @param GtkSelectionData *selectionData data being transfered
 * @param guint targetType Target Type
 * @param guint time Timestamp
 * @param gpointer userData Data being transfered
 */
void dragDataDelete( GtkWidget *widget,
                     GdkDragContext *context,
                     GtkSelectionData *selectionData,
                     guint targetType,
                     guint time,
                     gpointer userData);
                  
/**
 * @param GtkWidget *widget The widget being dropped on
 */
void dragDataReceived( GtkWidget *widget, 
                       GdkDragContext *context, 
                       gint x, 
                       gint y, 
                       GtkSelectionData *selectionData, 
                       guint targetType, 
                       guint time, 
                       gpointer userData );

int xx, yy;
void init( void );
void initStock( void );
void initTiles( void );
void unsetTile( int pos );
void isWidgetOnYPos( GtkWidget*, gpointer );
void isWidgetOnXPos( GtkWidget*, gpointer );
gboolean widgetOnYPosFound;
gboolean widgetOnXPosFound;
GtkWidget *getWidgetByYPos( int, int );
GtkWidget *getWidgetByXPos( int, int );
GtkWidget *widgetOnYPos;
GtkWidget *widgetOnXPos;
void deleteDestOnY( int );
gboolean isWidgetDragDest( GtkWidget* );
void moveDown( GtkWidget*, gpointer );
void moveRight( GtkWidget*, gpointer );
gboolean debug;
node *root;

#define typename(x) _Generic((x),        /* Get the name of a type */             \
                                                                                  \
        _Bool: "_Bool",                  unsigned char: "unsigned char",          \
         char: "char",                     signed char: "signed char",            \
    short int: "short int",         unsigned short int: "unsigned short int",     \
          int: "int",                     unsigned int: "unsigned int",           \
     long int: "long int",           unsigned long int: "unsigned long int",      \
long long int: "long long int", unsigned long long int: "unsigned long long int", \
        float: "float",                         double: "double",                 \
  long double: "long double",                   char *: "pointer to char",        \
       void *: "pointer to void",                int *: "pointer to int",         \
       GtkWidget *: "GtkWidget",\
      default: "other")
#endif

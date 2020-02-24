/**
 * MainWindow.c
 * Helper class for dealing with the main window of the application.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#include "MainWindow.h"

// Private variables.
GtkWidget *window;
GtkItemFactoryEntry menu_items[] = {
	// File.
	{ "/_File",                     NULL,             NULL,          0, "<Branch>",     NULL },
	{ "/File/_New",                 NULL,             NULL,          0, "<Branch>",     NULL },
	{ "/File/New/_Article...",      "<CTRL>N",        NULL,          0, "<StockItem>",  GTK_STOCK_NEW },
	{ "/File/New/_Template...",     NULL,             NULL,          0, "<Item>",       NULL },
	{ "/File/sep1",                 NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/File/_Open Workspace...",   "<CTRL>O",        NULL,          0, "<StockItem>",  GTK_STOCK_OPEN },
	{ "/File/_Refresh Workspace",   "<CTRL>R",        NULL,          0, "<StockItem>",  GTK_STOCK_REFRESH },
	{ "/File/_Close Workspace",     "<CTRL>W",        NULL,          0, "<StockItem>",  GTK_STOCK_CLOSE },
	{ "/File/sep2",                 NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/File/_Save",                "<CTRL>S",        NULL,          0, "<StockItem>",  GTK_STOCK_SAVE },
	{ "/File/Save _As...",          NULL,             NULL,          0, "<StockItem>",  GTK_STOCK_SAVE_AS },
	{ "/File/sep3",                 NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/File/_Quit",                "<CTRL>Q",        gtk_main_quit, 0, "<StockItem>",  GTK_STOCK_QUIT },
	// Edit.
	{ "/_Edit",                     NULL,             NULL,          0, "<Branch>",     NULL },
	{ "/Edit/_Undo",                "<CTRL>Z",        NULL,          0, "<StockItem>",  GTK_STOCK_UNDO },
	{ "/Edit/_Redo",                "<CTRL><SHIFT>Z", NULL,          0, "<StockItem>",  GTK_STOCK_REDO },
	{ "/Edit/sep1",                 NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/Edit/Cu_t",                 "<CTRL>X",        NULL,          0, "<StockItem>",  GTK_STOCK_CUT },
	{ "/Edit/_Copy",                "<CTRL>C",        NULL,          0, "<StockItem>",  GTK_STOCK_COPY },
	{ "/Edit/_Paste",               "<CTRL>V",        NULL,          0, "<StockItem>",  GTK_STOCK_PASTE },
	{ "/Edit/sep2",                 NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/Edit/Select A_ll",          "<CTRL>A",        NULL,          0, "<StockItem>",  GTK_STOCK_SELECT_ALL },
	// Search.
	{ "/_Search",                   NULL,             NULL,          0, "<Branch>",     NULL },
	{ "/Search/_Find...",           "<CTRL>F",        NULL,          0, "<StockItem>",  GTK_STOCK_FIND },
	{ "/Search/Find _Next",         "<CTRL>G",        NULL,          0, "<Item>",       NULL },
	{ "/Search/_Replace...",        "<CTRL>H",        NULL,          0, "<StockItem>",  GTK_STOCK_FIND_AND_REPLACE },
	{ "/Search/sep1",               NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/Search/Jump To Article...", "<CTRL>J",        NULL,          0, "<StockItem>",  GTK_STOCK_JUMP_TO },
	// View.
	{ "/_View",                     NULL,             NULL,          0, "<Branch>",     NULL },
	{ "/View/Page _Viewer",         NULL,             NULL,          0, "<StockItem>",  GTK_STOCK_INDEX },
	{ "/View/Page _Editor",         NULL,             NULL,          0, "<StockItem>",  GTK_STOCK_EDIT },
	{ "/View/sep1",                 NULL,             NULL,          0, "<Separator>",  NULL },
	{ "/View/_Toggle Page View",    "<CTRL>D",        NULL,          0, "<StockItem>",  GTK_STOCK_FIND_AND_REPLACE },
	// Help.
	{ "/_Help",                     NULL,             NULL,          0, "<LastBranch>", NULL },
	{ "/Help/_About",               NULL,             NULL,          0, "<StockItem>",  GTK_STOCK_ABOUT }
};

// Private methods.
GtkWidget* initialize_menubar();

/**
 * Initializes the main window of the application.
 */
void initialize_mainwindow() {
	GtkWidget *vbox;
	GtkWidget *menubar;

	// Create window and setup parameters.
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "gUki");
	g_signal_connect(window, "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	// Add vertical container to place the menu bar.
	vbox = gtk_vbox_new(false, 1);
	gtk_container_set_border_width (GTK_CONTAINER(vbox), 1);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	// Initialize the menu bar.
	menubar = initialize_menubar();
	gtk_box_pack_start(GTK_BOX(vbox), menubar, false, true, 0);

	// Show the window.
	gtk_widget_show_all(window);
}

/**
 * Initializes the menu bar.
 *
 * @return Menu bar widget already populated.
 */
GtkWidget* initialize_menubar() {
	GtkItemFactory *item_factory;
	GtkAccelGroup *accel_group;
	gint items_len;

	// Create an accelerator group and the item factory to place the menus.
	accel_group = gtk_accel_group_new();
	item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>",
										accel_group);

	// Create the menu items from the factory and attach the accelerators.
	items_len = sizeof(menu_items) / sizeof(GtkItemFactoryEntry);
	gtk_item_factory_create_items(item_factory, items_len, menu_items, NULL);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// Generate the menu bar and return.
	return gtk_item_factory_get_widget(item_factory, "<main>");
}

/**
 * MainWindow.c
 * Helper class for dealing with the main window of the application.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#include "MainWindow.h"
#include "Workspace.h"

// Private variables.
GtkWidget *window;
GtkWidget *statusbar;
GtkWidget *treeview;
GtkWidget *pageeditor;
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
GtkWidget* initialize_treeview();
GtkWidget* initialize_page_editor();

/**
 * Initializes the main window of the application.
 */
void initialize_mainwindow() {
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *hpaned;
	GtkWidget *sclwindow;

	// Create window and setup parameters.
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "gUki");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request(window, 700, 500);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Add vertical container to place the menu bar.
	vbox = gtk_vbox_new(false, 1);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 1);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	// Initialize the menu bar.
	menubar = initialize_menubar();
	gtk_box_pack_start(GTK_BOX(vbox), menubar, false, true, 0);

	// Add a horizontal panel.
	hpaned = gtk_hpaned_new();
	gtk_box_pack_start(GTK_BOX(vbox), hpaned, true, true, 0);

	// Initialize the tree view.
	treeview = initialize_treeview();
	gtk_widget_set_size_request(treeview, 200, -1);

	// Initialize the scrolled window that will contain the tree view.
	sclwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(sclwindow), treeview);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sclwindow),
								   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sclwindow),
										GTK_SHADOW_ETCHED_IN);
	gtk_paned_add1(GTK_PANED(hpaned), sclwindow);

	// Initialize the page editor.
	pageeditor = initialize_page_editor();
	gtk_paned_add2(GTK_PANED(hpaned), pageeditor);

	// Initialize the status bar.
	statusbar = gtk_statusbar_new();
	gtk_statusbar_push(GTK_STATUSBAR(statusbar),
	gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "Welcome to gUki"),
					   "Welcome to gUki");
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, false, true, 0);

	// Initialize the workspace.
	initialize_workspace(treeview);

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

/**
 * Initializes the tree view that will contain the articles and templates.
 *
 * @return Tree view widget to be populated.
 */
GtkWidget* initialize_treeview() {
	GtkWidget *tview;
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;

	// Create tree view.
	tview = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tview), false);

	// Create the only column
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Workspace");
	gtk_tree_view_append_column(GTK_TREE_VIEW(tview), col);

	// Create the cell renderer.
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

	return tview;
}

/**
 * Initializes the page editor (GtkTextView) control.
 *
 * @return The GtkTextView control.
 */
GtkWidget* initialize_page_editor() {
	GtkWidget *editor;

	// Create and setup editor.
	editor = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(editor), GTK_WRAP_WORD);

	return editor;
}

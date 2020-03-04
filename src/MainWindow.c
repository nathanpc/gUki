/**
 * MainWindow.c
 * Helper class for dealing with the main window of the application.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#include <uki/uki.h>
#include "MainWindow.h"
#include "DialogHelper.h"
#include "PageManager.h"
#include "Workspace.h"

// Private variables.
GtkWidget *window;
GtkWidget *statusbar;
GtkWidget *treeview;
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
void on_treeview_selection_changed(GtkWidget *widget, gpointer callback_data);
GtkWidget* initialize_menubar();
GtkWidget* initialize_treeview();
GtkWidget* initialize_notebook(GtkWidget *editor_container,
							   GtkWidget *viewer_container);

/**
 * Initializes the main window of the application.
 */
void initialize_mainwindow() {
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *hpaned;
	GtkWidget *scltree;
	GtkWidget *scleditor;
	GtkWidget *notebook;
	GtkWidget *pageeditor;
	GtkWidget *pageviewer;

	// Create window and setup parameters.
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "gUki");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request(window, 700, 500);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Initialize dialogs.
	initialize_dialogs(window);

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
	scltree = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scltree), treeview);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scltree),
								   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scltree),
										GTK_SHADOW_ETCHED_IN);
	gtk_paned_add1(GTK_PANED(hpaned), scltree);

	// Initialize the page manager.
	initialize_page_manager(&pageeditor, &pageviewer);

	// Initialize the scrolled window that will contain the page editor.
	scleditor = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scleditor), pageeditor);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scleditor),
								   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scleditor),
										GTK_SHADOW_ETCHED_IN);

	// Initialize the notebook that will hold the page viewer and editor.
	notebook = initialize_notebook(scleditor, pageviewer);
	gtk_paned_add2(GTK_PANED(hpaned), notebook);

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
 * Callback for the tree view selection changed signal.
 *
 * @param widget        The widget that received the signal.
 * @param callback_data Data passed to us by the signal connector.
 */
void on_treeview_selection_changed(GtkWidget *widget, gpointer callback_data) {
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkTreeSelection *selection;

	selection = GTK_TREE_SELECTION(widget);
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		gint index;
		gchar type;

		// Get the important values from the selection.
		gtk_tree_model_get(model, &iter, COL_INDEX, &index, COL_TYPE, &type, -1);

		// Check for the type of selection.
		switch (type) {
		case ROW_TYPE_ARTICLE:
			load_article(index);
			break;
		case ROW_TYPE_TEMPLATE:
			load_template(index);
			break;
		}
	}
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
	GtkTreeViewColumn *mcol;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;

	// Create tree view.
	tview = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tview), false);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tview));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	// Create the main column
	mcol = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(mcol, "Workspace");
	gtk_tree_view_append_column(GTK_TREE_VIEW(tview), mcol);

	// Create the cell renderer.
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(mcol, renderer, TRUE);
	gtk_tree_view_column_add_attribute(mcol, renderer, "text", 0);

	// Set the callback for the selection change signal.
	g_signal_connect(selection, "changed",
					 G_CALLBACK(on_treeview_selection_changed), NULL);

	return tview;
}

/**
 * Initializes the notebook widget that will hold the page editor and viewer.
 *
 * @param  editor_container Widget that contains the page editor.
 * @param  viewer_container Widget that contains the page viewer.
 * @return                  The GtkNotebook widget.
 */
GtkWidget* initialize_notebook(GtkWidget *editor_container,
							   GtkWidget *viewer_container) {
	GtkWidget *notebook;
	GtkWidget *lblview;
	GtkWidget *lbledit;

	// Initialize the notebook widget.
	notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

	// Set the labels.
	lblview = gtk_label_new("Viewer");
	lbledit = gtk_label_new("Editor");

	// Append widgets to the notebook.
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), viewer_container, lblview);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), editor_container, lbledit);

	// Set current page to be the first one and return.
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
	return notebook;
}

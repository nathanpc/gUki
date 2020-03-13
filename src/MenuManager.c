/**
 * MenuManager.c
 * A helper module to manage the menus and toolbar of the application.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <stdarg.h>
#include "MenuManager.h"
#include "PageManager.h"
#include "Workspace.h"
#include "MainWindow.h"

// Private variables.
GtkItemFactory *main_menu_factory;
GtkWidget *window;

// Menu items.
GtkItemFactoryEntry menu_items[] = {
	// File.
	{ "/_File",                     NULL,             NULL,                          0, "<Branch>",     NULL },
	{ "/File/_New",                 NULL,             NULL,                          0, "<Branch>",     NULL },
	{ "/File/New/_Article...",      "<CTRL>N",        menu_new_page,                 1, "<StockItem>",  GTK_STOCK_NEW },
	{ "/File/New/_Template...",     NULL,             menu_new_page,                 2, "<Item>",       NULL },
	{ "/File/sep1",                 NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/File/_Open Workspace...",   "<CTRL>O",        workspace_open,                0, "<StockItem>",  GTK_STOCK_OPEN },
	{ "/File/_Refresh Workspace",   "<CTRL>R",        workspace_refresh,             0, "<StockItem>",  GTK_STOCK_REFRESH },
	{ "/File/_Close Workspace",     "<CTRL>W",        workspace_close,               0, "<StockItem>",  GTK_STOCK_CLOSE },
	{ "/File/sep2",                 NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/File/_Save",                "<CTRL>S",        page_save,                     0, "<StockItem>",  GTK_STOCK_SAVE },
	{ "/File/Save _As...",          NULL,             page_save_as,                  0, "<StockItem>",  GTK_STOCK_SAVE_AS },
	{ "/File/sep3",                 NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/File/_Quit",                "<CTRL>Q",        window_destroy,                0, "<StockItem>",  GTK_STOCK_QUIT },
	// Edit.
	{ "/_Edit",                     NULL,             NULL,                          0, "<Branch>",     NULL },
	{ "/Edit/_Undo",                "<CTRL>Z",        NULL,                          0, "<StockItem>",  GTK_STOCK_UNDO },
	{ "/Edit/_Redo",                "<CTRL><SHIFT>Z", NULL,                          0, "<StockItem>",  GTK_STOCK_REDO },
	{ "/Edit/sep1",                 NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/Edit/Cu_t",                 "<CTRL>X",        editor_cut,                    0, "<StockItem>",  GTK_STOCK_CUT },
	{ "/Edit/_Copy",                "<CTRL>C",        editor_copy,                   0, "<StockItem>",  GTK_STOCK_COPY },
	{ "/Edit/_Paste",               "<CTRL>V",        editor_paste,                  0, "<StockItem>",  GTK_STOCK_PASTE },
	{ "/Edit/sep2",                 NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/Edit/Select A_ll",          "<CTRL>A",        editor_select_all,             0, "<StockItem>",  GTK_STOCK_SELECT_ALL },
	// Search.
	{ "/_Search",                   NULL,             NULL,                          0, "<Branch>",     NULL },
	{ "/Search/_Find...",           "<CTRL>F",        show_dialog_find,              0, "<StockItem>",  GTK_STOCK_FIND },
	{ "/Search/Find _Next",         "<CTRL>G",        editor_find_next,              0, "<Item>",       NULL },
	{ "/Search/_Replace...",        "<CTRL>H",        NULL,                          0, "<StockItem>",  GTK_STOCK_FIND_AND_REPLACE },
	{ "/Search/sep1",               NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/Search/Jump To Article...", "<CTRL>J",        NULL,                          0, "<StockItem>",  GTK_STOCK_JUMP_TO },
	// View.
	{ "/_View",                     NULL,             NULL,                          0, "<Branch>",     NULL },
	{ "/View/Page _Viewer",         NULL,             show_page_viewer,              0, "<StockItem>",  GTK_STOCK_INDEX },
	{ "/View/Page _Editor",         NULL,             show_page_editor,              0, "<StockItem>",  GTK_STOCK_EDIT },
	{ "/View/sep1",                 NULL,             NULL,                          0, "<Separator>",  NULL },
	{ "/View/_Toggle Page View",    "<CTRL>D",        toggle_notebook_page,          0, "<StockItem>",  GTK_STOCK_FIND_AND_REPLACE },
	// Help.
	{ "/_Help",                     NULL,             NULL,                          0, "<Branch>",     NULL },
	{ "/Help/_About",               NULL,             show_about,                    0, "<StockItem>",  GTK_STOCK_ABOUT }
};

/**
 * Initializes te menu manager.
 *
 * @param parent_window Parent window widget.
 */
void initialize_menu_manager(GtkWidget *parent_window) {
	window = parent_window;
}

/**
 * Initializes the menu bar.
 *
 * @return Menu bar widget already populated.
 */
GtkWidget* initialize_menubar() {
	GtkAccelGroup *accel_group;
	gint items_len;

	// Create an accelerator group and the item factory to place the menus.
	accel_group = gtk_accel_group_new();
	main_menu_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>",
											 accel_group);

	// Create the menu items from the factory and attach the accelerators.
	items_len = sizeof(menu_items) / sizeof(GtkItemFactoryEntry);
	gtk_item_factory_create_items(main_menu_factory, items_len, menu_items, NULL);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// Generate the menu bar and return.
	return gtk_item_factory_get_widget(main_menu_factory, "<main>");
}

/**
 * Initializes the toolbar widget.
 *
 * @return Toolbar widget populated and ready.
 */
GtkWidget* initialize_toolbar() {
	GtkWidget *toolbar;
	GtkToolItem *item;

	// Create the toolbar.
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	// Add the page items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_JUMP_TO);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	
	// Add the workspace items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);

	// Add the usual edit items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_CUT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_PASTE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);

	// Add the search items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_FIND);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_FIND_AND_REPLACE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);

	return toolbar;
}

/**
 * Updates the state the widgets that are state-sensitive.
 */
void update_workspace_state_menu() {
	GtkWidget *menu_refresh_workspace;
	GtkWidget *menu_close_workspace;
	GtkWidget *menu_new_templace;
	GtkWidget *menu_new_article;
	GtkWidget *menu_save_as;
	GtkWidget *menu_save;
	GtkWidget *menu_jump_article;

	// Get the menu item widgets.
	menu_refresh_workspace = gtk_item_factory_get_widget(main_menu_factory,
			"/File/Refresh Workspace");
	menu_close_workspace = gtk_item_factory_get_widget(main_menu_factory, 
			"/File/Close Workspace");
	menu_new_templace = gtk_item_factory_get_widget(main_menu_factory,
			"/File/New/Template...");
	menu_new_article = gtk_item_factory_get_widget(main_menu_factory,
			"/File/New/Article...");
	menu_save_as = gtk_item_factory_get_widget(main_menu_factory,
			"/File/Save As...");
	menu_save = gtk_item_factory_get_widget(main_menu_factory,
			"/File/Save");
	menu_jump_article = gtk_item_factory_get_widget(main_menu_factory,
			"/Search/Jump To Article...");

	// Handle workspace change.
	if (is_workspace_opened()) {
		gtk_widget_set_sensitive(menu_refresh_workspace, true);
		gtk_widget_set_sensitive(menu_close_workspace, true);
		gtk_widget_set_sensitive(menu_new_article, true);
		gtk_widget_set_sensitive(menu_new_templace, true);
		gtk_widget_set_sensitive(menu_jump_article, true);
	} else {
		gtk_widget_set_sensitive(menu_refresh_workspace, false);
		gtk_widget_set_sensitive(menu_close_workspace, false);
		gtk_widget_set_sensitive(menu_new_article, false);
		gtk_widget_set_sensitive(menu_new_templace, false);
		gtk_widget_set_sensitive(menu_jump_article, false);
	}

	// Handle article change.
	if (is_article_opened()) {
		gtk_widget_set_sensitive(menu_save, true);
		gtk_widget_set_sensitive(menu_save_as, true);
	} else {
		gtk_widget_set_sensitive(menu_save, false);
		gtk_widget_set_sensitive(menu_save_as, false);
	}
}



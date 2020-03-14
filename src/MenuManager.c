/**
 * MenuManager.c
 * A helper module to manage the menus and toolbar of the application.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <gdk/gdkkeysyms.h>
#include <stdarg.h>
#include "MenuManager.h"
#include "PageManager.h"
#include "Workspace.h"
#include "MainWindow.h"

// Private variables.
GtkWidget *window;
GtkWidget *menubar;

// Global toolbar items.
GtkToolItem *tool_new_page;
GtkToolItem *tool_save_page;
GtkToolItem *tool_jump_to;
GtkToolItem *tool_workspace_refresh;
GtkToolItem *tool_workspace_close;

// Global menu items.
GtkWidget *menu_refresh_workspace;
GtkWidget *menu_close_workspace;
GtkWidget *menu_new_template;
GtkWidget *menu_new_article;
GtkWidget *menu_save_as;
GtkWidget *menu_save;
GtkWidget *menu_jump_page;

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
	GtkAccelGroup *accel_group = NULL;
	GtkWidget *menu;
	GtkWidget *menu_file;
	GtkWidget *menu_edit;
	GtkWidget *menu_search;
	GtkWidget *menu_view;
	GtkWidget *menu_help;
	GtkWidget *submenu;
	GtkWidget *subitem;
	GtkWidget *item;
	GtkWidget *separator;

	// Create the menu bar and accelerator group.
	menubar = gtk_menu_bar_new();
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// Build the file menu.
	menu = gtk_menu_new();
	menu_file = gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_file), menu);
	submenu = gtk_menu_new();
	subitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(subitem), submenu);
	menu_new_article = gtk_menu_item_new_with_label("Article...");
	gtk_widget_add_accelerator(menu_new_article, "activate", accel_group,
			GDK_KEY_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(menu_new_article), "activate",
			G_CALLBACK(on_menu_new_page), (void*)(long)1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_new_article);
	menu_new_template = gtk_menu_item_new_with_label("Template...");
	g_signal_connect(G_OBJECT(menu_new_template), "activate",
			G_CALLBACK(on_menu_new_page), (void*)(long)2);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_new_template);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), subitem);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, accel_group);
	gtk_menu_item_set_label(GTK_MENU_ITEM(item), "Open Workspace...");
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(on_workspace_open),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	menu_refresh_workspace = gtk_image_menu_item_new_from_stock(
			GTK_STOCK_REFRESH, accel_group);
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_refresh_workspace),
			"Refresh Workspace");
	//gtk_widget_add_accelerator(menu_refresh_workspace, "activate", accel_group,
	//		GDK_r, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(menu_refresh_workspace), "activate",
			G_CALLBACK(on_workspace_refresh), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_refresh_workspace);
	menu_close_workspace = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE,
			accel_group);
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_close_workspace),
			"Close Workspace");
	g_signal_connect(G_OBJECT(menu_close_workspace), "activate",
			G_CALLBACK(on_workspace_close), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_close_workspace);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	menu_save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, accel_group);
	g_signal_connect(G_OBJECT(menu_save), "activate", G_CALLBACK(on_page_save),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_save);
	menu_save_as = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
	g_signal_connect(G_OBJECT(menu_save_as), "activate",
			G_CALLBACK(on_page_save_as), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_save_as);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(window_destroy),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_file);

	// Build the edit menu.
	menu = gtk_menu_new();
	menu_edit = gtk_menu_item_new_with_label("Edit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_edit), menu);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_UNDO, accel_group);
	gtk_widget_add_accelerator(item, "activate", accel_group,
			GDK_KEY_z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(on_editor_undo),
	//		NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_REDO, accel_group);
	gtk_widget_add_accelerator(item, "activate", accel_group,
			GDK_KEY_z, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
	//g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(on_editor_redo),
	//		NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, accel_group);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(on_editor_cut),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, accel_group);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(on_editor_copy),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, accel_group);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(on_editor_paste),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_ALL,
			accel_group);
	gtk_widget_add_accelerator(item, "activate", accel_group,
			GDK_KEY_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_editor_select_all), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_edit);

	// Build the search menu.
	menu = gtk_menu_new();
	menu_search = gtk_menu_item_new_with_label("Search");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_search), menu);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND, accel_group);
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_show_dialog_find), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	item = gtk_menu_item_new_with_label("Find Next");
	gtk_widget_add_accelerator(item, "activate", accel_group,
			GDK_KEY_g, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_editor_find_next), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND_AND_REPLACE,
			accel_group);
	gtk_widget_add_accelerator(item, "activate", accel_group,
			GDK_KEY_h, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//g_signal_connect(G_OBJECT(item), "activate",
	//		G_CALLBACK(on_show_dialog_find), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	menu_jump_page = gtk_image_menu_item_new_from_stock(GTK_STOCK_JUMP_TO,
			accel_group);
	gtk_menu_item_set_label(GTK_MENU_ITEM(menu_jump_page), "Jump To Page...");
	gtk_widget_add_accelerator(menu_jump_page, "activate", accel_group,
			GDK_KEY_j, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//g_signal_connect(G_OBJECT(menu_jump_page), "activate",
	//		G_CALLBACK(on_jump_to_page), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_jump_page);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_search);

	// Build the view menu.
	menu = gtk_menu_new();
	menu_view = gtk_menu_item_new_with_label("View");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_view), menu);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_INDEX, NULL);
	gtk_menu_item_set_label(GTK_MENU_ITEM(item), "Page Viewer");
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_show_page_viewer), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_EDIT, NULL);
	gtk_menu_item_set_label(GTK_MENU_ITEM(item), "Page Editor");
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_show_page_editor), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	separator = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);
	item = gtk_menu_item_new_with_label("Toggle Page View");
	gtk_widget_add_accelerator(item, "activate", accel_group,
			GDK_KEY_d, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_toggle_notebook_page), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_view);

	// Build the help menu.
	menu = gtk_menu_new();
	menu_help = gtk_menu_item_new_with_label("Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_help), menu);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, accel_group);
	g_signal_connect(G_OBJECT(item), "activate",
			G_CALLBACK(on_show_about), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_help);

	return menubar;
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
	tool_new_page = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	g_signal_connect(tool_new_page, "clicked", G_CALLBACK(on_menu_new_page),
			NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_new_page, -1);
	tool_save_page = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	g_signal_connect(tool_save_page, "clicked", G_CALLBACK(on_page_save),
			NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_save_page, -1);
	tool_jump_to = gtk_tool_button_new_from_stock(GTK_STOCK_JUMP_TO);
	//g_signal_connect(tool_jump_to, "clicked", G_CALLBACK(on_jump_to_page),
	//		NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_jump_to, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	
	// Add the workspace items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	g_signal_connect(item, "clicked", G_CALLBACK(on_workspace_open), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	tool_workspace_refresh = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
	g_signal_connect(tool_workspace_refresh, "clicked",
			G_CALLBACK(on_workspace_refresh), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_workspace_refresh, -1);
	tool_workspace_close = gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE);
	g_signal_connect(tool_workspace_close, "clicked",
			G_CALLBACK(on_workspace_close), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_workspace_close, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);

	// Add the usual edit items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_CUT);
	g_signal_connect(item, "clicked", G_CALLBACK(on_editor_cut), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
	g_signal_connect(item, "clicked", G_CALLBACK(on_editor_copy), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_PASTE);
	g_signal_connect(item, "clicked", G_CALLBACK(on_editor_paste), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);

	// Add the search items.
	item = gtk_tool_button_new_from_stock(GTK_STOCK_FIND);
	g_signal_connect(item, "clicked", G_CALLBACK(on_show_dialog_find), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_tool_button_new_from_stock(GTK_STOCK_FIND_AND_REPLACE);
	//g_signal_connect(item, "clicked", G_CALLBACK(on_show_dialog_replace),
	//		NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	item = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);

	return toolbar;
}

/**
 * Updates the state the widgets that are state-sensitive.
 */
void update_workspace_state_menu() {
	// Handle workspace change.
	if (is_workspace_opened()) {
		// Menu items.
		gtk_widget_set_sensitive(menu_refresh_workspace, true);
		gtk_widget_set_sensitive(menu_close_workspace, true);
		gtk_widget_set_sensitive(menu_new_article, true);
		gtk_widget_set_sensitive(menu_new_template, true);
		gtk_widget_set_sensitive(menu_jump_page, true);

		// Toolbar items.
		gtk_widget_set_sensitive(GTK_WIDGET(tool_workspace_refresh), true);
		gtk_widget_set_sensitive(GTK_WIDGET(tool_workspace_close), true);
		gtk_widget_set_sensitive(GTK_WIDGET(tool_new_page), true);
		gtk_widget_set_sensitive(GTK_WIDGET(tool_jump_to), true);
	} else {
		// Menu items.
		gtk_widget_set_sensitive(menu_refresh_workspace, false);
		gtk_widget_set_sensitive(menu_close_workspace, false);
		gtk_widget_set_sensitive(menu_new_article, false);
		gtk_widget_set_sensitive(menu_new_template, false);
		gtk_widget_set_sensitive(menu_jump_page, false);
		
		// Toolbar items.
		gtk_widget_set_sensitive(GTK_WIDGET(tool_workspace_refresh), false);
		gtk_widget_set_sensitive(GTK_WIDGET(tool_workspace_close), false);
		gtk_widget_set_sensitive(GTK_WIDGET(tool_new_page), false);
		gtk_widget_set_sensitive(GTK_WIDGET(tool_jump_to), false);
	}

	// Handle article change.
	if (is_article_opened()) {
		// Menu items.
		gtk_widget_set_sensitive(menu_save, true);
		gtk_widget_set_sensitive(menu_save_as, true);

		// Toolbar items.
		gtk_widget_set_sensitive(GTK_WIDGET(tool_save_page), true);
	} else {
		// Menu items.
		gtk_widget_set_sensitive(menu_save, false);
		gtk_widget_set_sensitive(menu_save_as, false);

		// Toolbar items.
		gtk_widget_set_sensitive(GTK_WIDGET(tool_save_page), false);
	}
}


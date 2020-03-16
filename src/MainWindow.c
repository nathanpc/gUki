/**
 * MainWindow.c
 * Helper class for dealing with the main window of the application.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <uki/uki.h>
#include <glib/gstdio.h>
#include "MainWindow.h"
#include "MenuManager.h"
#include "AppProperties.h"
#include "DialogHelper.h"
#include "FindReplace.h"
#include "PageManager.h"
#include "Workspace.h"

// Private variables.
GtkWidget *window;
GtkWidget *treeview;
GtkWidget *notebook;

// Private methods.
GtkWidget* initialize_treeview();
GtkWidget* initialize_notebook(GtkWidget *editor_container,
							   GtkWidget *viewer_container);

/**
 * Initializes the main window of the application.
 */
void initialize_mainwindow() {
	GtkTextBuffer *editor_buffer;
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *toolbar;
	GtkWidget *hpaned;
	GtkWidget *scltree;
	GtkWidget *scleditor;
	GtkWidget *pageeditor;
	GtkWidget *pageviewer;
	gint window_width;

	// Create window and setup parameters.
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), APP_NAME);
	gtk_window_set_icon_name(GTK_WINDOW(window), APP_ICON);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request(window, 700, 500);
	g_signal_connect(window, "delete_event", G_CALLBACK(on_window_delete), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(window_destroy), NULL);

	// Get window width for relative width stuff.
	gtk_window_get_size(GTK_WINDOW(window), &window_width, NULL);

	// Initialize dialogs.
	initialize_dialogs(window);

	// Add vertical container to place the menu bar.
#if GTK_MAJOR_VERSION == 2
	vbox = gtk_vbox_new(false, 1);
#else
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
#endif
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 1);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	// Initialize the menu bar and the tool bar.
	initialize_menu_manager(window);
	menubar = initialize_menubar();
	gtk_box_pack_start(GTK_BOX(vbox), menubar, false, true, 0);
#if GTK_MAJOR_VERSION == 2
	toolbar = initialize_toolbar();
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, false, true, 0);
#endif

	// Add a horizontal panel.
#if GTK_MAJOR_VERSION == 2
	hpaned = gtk_hpaned_new();
#else
	hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
#endif
	gtk_box_pack_start(GTK_BOX(vbox), hpaned, true, true, 0);

	// Initialize the tree view.
	treeview = initialize_treeview();

	// Initialize the scrolled window that will contain the tree view.
	scltree = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request(scltree, window_width / 3, -1);
	gtk_container_add(GTK_CONTAINER(scltree), treeview);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scltree),
								   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scltree),
										GTK_SHADOW_ETCHED_IN);
	gtk_paned_add1(GTK_PANED(hpaned), scltree);

	// Initialize the page manager and the find and replace module.
	initialize_page_manager(&pageeditor, &pageviewer);
	initialize_find_replace(window, pageeditor);

	// Set the page editor text buffer changed signal callback.
	editor_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pageeditor));
    g_signal_connect(editor_buffer, "changed",
					 G_CALLBACK(on_editor_buffer_changed), NULL);

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

	// Initialize the workspace.
	initialize_workspace(treeview);

	// Show the window.
	gtk_widget_show_all(window);

	// Set the state of the widgets affected by the changes.
	update_workspace_state_menu();
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
	g_signal_connect(tview, "button-press-event",
					 G_CALLBACK(on_treeview_button_pressed), NULL);
    g_signal_connect(tview, "popup-menu", G_CALLBACK(on_treeview_popup_menu),
					 NULL);

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
	guint viewer_page_index = 0;

	// Initialize the notebook widget.
	notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

	// Set the labels.
	lblview = gtk_label_new("Viewer");
	lbledit = gtk_label_new("Editor");

	// Append widgets to the notebook.
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), viewer_container, lblview);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), editor_container, lbledit);

	// Set current page to be the first one.
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), viewer_page_index);

	// Set the callback for the selection change signal.
	g_signal_connect(notebook, "switch-page",
					 G_CALLBACK(on_notebook_page_changed),
					 (void*)(long)viewer_page_index);

	return notebook;
}

/**
 * Destroys the main window
 */
void window_destroy() {
	// Call the window deletion callback just to be sure.
	on_window_delete();

	// Clean up.
	destroy_find_replace();
	close_workspace();

	// Quit the GTK loop.
	gtk_main_quit();
}

/**
 * Callback function for the window deletionsignal.
 */
void on_window_delete() {
	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;
}

/**
 * Callback for the page editor text buffer changed signal.
 *
 * @param buffer    The text buffer that originated the signal.
 * @param user_data Data passed by the signal connector.
 */
void on_editor_buffer_changed(GtkTextBuffer *buffer, gpointer user_data) {
	// Set the page as having unsaved changes.
	set_page_unsaved_changes(true);
}

/**
 * Callback for the notebook page changed signal.
 *
 * @param notebook  The notebook that received the signal.
 * @param page      The new current widget.
 * @param page_num  The index of the new page.
 * @param user_data Data passed by the signal connector. In this case is the
 *                  viewer page index.
 */
void on_notebook_page_changed(GtkNotebook *notebook, GtkWidget *page,
							  guint page_num, gpointer user_data) {
	// Get the viewer page index from the user_data field.
	guint view_index = (unsigned int)(long)user_data;

	// Only do something if we are changing to the viewer tab.
	if (page_num == view_index) {
		refresh_page_viewer();
	}
}

/**
 * Callback for the tree view selection changed signal.
 *
 * @param widget        The widget that received the signal.
 * @param callback_data Data passed to us by the signal connector.
 */
void on_treeview_selection_changed(GtkWidget *widget, gpointer callback_data) {
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;

	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;

	// Get the selected item.
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
		default:
			clear_page_contents();
			break;
		}
	}

	// Set the state of the widgets affected by the changes.
	update_workspace_state_menu();
}

/**
 * Callback for the tree view button pressed signal.
 *
 * @param widget   The widget that received the signal.
 * @param event    The event structure containing more information about it.
 * @param userdata Data passed to us by the signal connector.
 */
gboolean on_treeview_button_pressed(GtkWidget *widget, GdkEventButton *event,
									gpointer userdata) {
	// Check that we had a right button single click event.
	if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
		GtkTreeSelection *selection;
		GtkTreePath *path;

		// Get selection object and the path to the selected item.
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
		if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
										  event->x, event->y,
										  &path, NULL, NULL, NULL)) {
			// Unselect everything and select the one we are clicking at.
			gtk_tree_selection_unselect_all(selection);
			gtk_tree_selection_select_path(selection, path);

			// Free the path.
			gtk_tree_path_free(path);
		}

		// Everything is fine. We handled this.
		return on_treeview_popup_menu(widget, event, userdata);
	}

	// Notify that we haven't handled this event.
	return false;
}

/**
 * Callback for the tree view popup menu signal.
 *
 * @param widget   The widget that received the signal.
 * @param event    The event structure containing more information about it.
 * @param userdata Data passed to us by the signal connector.
 */
gboolean on_treeview_popup_menu(GtkWidget *widget, GdkEventButton *event,
								gpointer userdata) {
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;

	// Get the selected item.
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		gchar type;

		// Get the type of the selected item and only proceed if it should.
		gtk_tree_model_get(model, &iter, COL_TYPE, &type, -1);
		if ((type == ROW_TYPE_ARTICLE) || (type == ROW_TYPE_TEMPLATE)) {
			GtkWidget *menu;
			GtkWidget *menuitem;

			// Create the popup menu.
			menu = gtk_menu_new();

			// Create the Delete menu item.
#if GTK_MAJOR_VERSION == 2
			menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_DELETE,
					NULL);
#else
			menuitem = gtk_menu_item_new_with_mnemonic("Delete");
#endif
			g_signal_connect(menuitem, "activate",
							 G_CALLBACK(on_treeview_popup_menu_click_delete),
							 widget);
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

			// Show the popup menu.
			gtk_widget_show_all(menu);
			gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
						   (event != NULL) ? event->button : 0,
						   gdk_event_get_time((GdkEvent*)event));

			// We handled this.
			return true;
		}
	}

	// We didn't handle this.
    return false;
}

/**
 * Callback for the tree view popup menu Delete clicked signal.
 *
 * @param widget   The widget that received the signal.
 * @param userdata Data passed to us by the signal connector.
 */
void on_treeview_popup_menu_click_delete(GtkWidget *widget, gpointer userdata) {
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;

	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;

	// Get the selected item.
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(userdata));
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		uki_error uki_err;
		char fpath[UKI_MAX_PATH];
		gint index;
		gchar type;

		// Get the important values from the selection.
		gtk_tree_model_get(model, &iter, COL_INDEX, &index, COL_TYPE, &type, -1);

		// Check for the type of selection.
		if (type == ROW_TYPE_ARTICLE) {
			// Get the article
			uki_article_t article = uki_article((size_t)index);

			// Get the file path.
			if ((uki_err = uki_article_fpath(fpath, article)) != UKI_OK) {
				error_dialog("Path Error", "Unable to find path for article '%s'.",
							 article.name);
				return;
			}
		} else if (type == ROW_TYPE_TEMPLATE) {
			// Get the template.
			uki_template_t template = uki_template((size_t)index);

			// Get the file path.
			if ((uki_err = uki_template_fpath(fpath, template)) != UKI_OK) {
				error_dialog("Path Error", "Unable to find path for template '%s'.",
							 template.name);
				return;
			}
		}

		// Remove the file.
		if (g_unlink(fpath))
			error_dialog("Delete Error", "An error occured while trying to "
						 "delete the file '%s'.", fpath);

		// Reload the workspace.
		reload_workspace();
	}
}

/**
 * Menu item callback for creating a new page.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_menu_new_page(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	GtkFileFilter *filter;
	size_t ub_len;
	gint res;
	char *uri;
	char fpath[UKI_MAX_PATH];
	bool is_article;

	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;

	// Set some state variables.
	ub_len = sizeof("file://");
	is_article = ((unsigned int)(long)data == 1);

	// Check if we have an article or template and setup accordingly.
	if (is_article) {
		uki_folder_articles(fpath);
	} else {
		uki_folder_templates(fpath);
	}

	// Allocate and build the URI string.
	uri = (char*)malloc((ub_len + strlen(fpath) + 1) * sizeof(char));
	sprintf(uri, "file://%s", fpath);

	// Create the save dialog and set it up.
	dialog = gtk_file_chooser_dialog_new("New Page", GTK_WINDOW(window),
										 GTK_FILE_CHOOSER_ACTION_SAVE,
#if GTK_MAJOR_VERSION == 2
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										 GTK_STOCK_NEW, GTK_RESPONSE_OK,
#else
										 "Cancel", GTK_RESPONSE_CANCEL,
										 "New", GTK_RESPONSE_OK,
#endif
										 NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(dialog), uri);
	gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(dialog), true);
	gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dialog), true);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),
									  "." UKI_ARTICLE_EXT);

	// Set the file filters.
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "HTML File");
	gtk_file_filter_add_pattern(filter, "*." UKI_ARTICLE_EXT);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "All Files");
	gtk_file_filter_add_pattern(filter, "*.*");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);

	// Show the dialog.
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res != GTK_RESPONSE_OK) {
		gtk_widget_destroy(dialog);
		return;
	}

	// Get the file name and destroy the dialog.
	free(uri);
	uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog));
	strcpy(fpath, uri + ub_len - 1);
	g_free(uri);
	gtk_widget_destroy(dialog);

	// Create a new page.
	if (is_article) {
		new_article(fpath);
	} else {
		new_template(fpath);
	}

	// Make sure we have a blank file to save.
	clear_page_contents();

	// Save the new current page and reload the workspace.
	save_current_page();
	reload_workspace();

	// Set the state of the widgets affected by the changes.
	update_workspace_state_menu();
}

/**
 * Menu item callback for opening a new workspace.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_workspace_open(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	size_t ub_len;
	gint res;
	char *uri;
	char fpath[UKI_MAX_PATH];
	ub_len = sizeof("file://");

	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;

	// Close the current workspace.
	close_workspace();

	// Create the open folder dialog and set it up.
	dialog = gtk_file_chooser_dialog_new("Open Uki Workspace Folder",
										 GTK_WINDOW(window),
										 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
#if GTK_MAJOR_VERSION == 2
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										 GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
#else
										 "Cancel", GTK_RESPONSE_CANCEL,
										 "Open", GTK_RESPONSE_OK, NULL);
#endif
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(dialog), true);
	gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dialog), true);

	// Show the dialog.
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res != GTK_RESPONSE_OK) {
		gtk_widget_destroy(dialog);
		return;
	}

	// Get the file name and destroy the dialog.
	uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog));
	strncpy(fpath, uri + ub_len - 1, UKI_MAX_PATH - 1);
	g_free(uri);
	gtk_widget_destroy(dialog);

	// Open the new workspace.
	open_workspace(fpath);

	// Set the state of the widgets affected by the changes.
	update_workspace_state_menu();
}

/**
 * Menu item callback for refreshing the workspace.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_workspace_refresh(GtkWidget *widget, gpointer data) {
	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;

	// Actually reload the workspace.
	reload_workspace();

	// Set the state of the widgets affected by the changes.
	update_workspace_state_menu();
}

/**
 * Menu item callback for closing the workspace.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_workspace_close(GtkWidget *widget, gpointer data) {
	// Check if we have unsaved changes.
	if (check_page_unsaved_changes())
		return;

	// Actually close the workspace.
	close_workspace();

	// Set the state of the widgets affected by the changes.
	update_workspace_state_menu();
}

/**
 * Menu item callback for saving the current opened page.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_page_save(GtkWidget *widget, gpointer data) {
	save_current_page();
}

/**
 * Menu item callback for saving the current opened page as a new page.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_page_save_as(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	GtkFileFilter *filter;
	size_t ub_len;
	gint res;
	char *uri;
	char fpath[UKI_MAX_PATH];
	ub_len = sizeof("file://");

	// Check if we have an article or template and setup accordingly.
	if (is_article_opened()) {
		uki_folder_articles(fpath);
	} else {
		uki_folder_templates(fpath);
	}

	// Allocate and build the URI string.
	uri = (char*)malloc((ub_len + strlen(fpath) + 1) * sizeof(char));
	sprintf(uri, "file://%s", fpath);

	// Create the save dialog and set it up.
	dialog = gtk_file_chooser_dialog_new("Save Page As", GTK_WINDOW(window),
										 GTK_FILE_CHOOSER_ACTION_SAVE,
#if GTK_MAJOR_VERSION == 2
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										 GTK_STOCK_SAVE_AS, GTK_RESPONSE_OK,
#else
										 "Cancel", GTK_RESPONSE_CANCEL,
										 "Save As", GTK_RESPONSE_OK,
#endif
										 NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	gtk_file_chooser_set_current_folder_uri(GTK_FILE_CHOOSER(dialog), uri);
	gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(dialog), true);
	gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dialog), true);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),
									  "." UKI_ARTICLE_EXT);

	// Set the file filters.
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "HTML File");
	gtk_file_filter_add_pattern(filter, "*." UKI_ARTICLE_EXT);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "All Files");
	gtk_file_filter_add_pattern(filter, "*.*");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);

	// Show the dialog.
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res != GTK_RESPONSE_OK) {
		gtk_widget_destroy(dialog);
		return;
	}

	// Get the file name and destroy the dialog.
	free(uri);
	uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog));
	strcpy(fpath, uri + ub_len - 1);
	g_free(uri);
	gtk_widget_destroy(dialog);

	// Create a new page.
	if (is_article_opened()) {
		new_article(fpath);
	} else {
		new_template(fpath);
	}

	// Save the new current page and reload the workspace.
	save_current_page();
	reload_workspace();
}

/**
 * Menu item callback for cutting the text in the editor.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_editor_cut(GtkWidget *widget, gpointer data) {
	GtkClipboard *clipboard;
	GtkTextBuffer *buffer;

	// Get the text buffer and the default clipboard.
	buffer = get_page_editor_buffer();
	clipboard = gtk_clipboard_get(GDK_NONE);

	// Actually cut the text.
	gtk_text_buffer_cut_clipboard(buffer, clipboard, true);
}

/**
 * Menu item callback for copying the text in the editor.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_editor_copy(GtkWidget *widget, gpointer data) {
	GtkClipboard *clipboard;
	GtkTextBuffer *buffer;

	// Get the text buffer and the default clipboard.
	buffer = get_page_editor_buffer();
	clipboard = gtk_clipboard_get(GDK_NONE);

	// Actually copy the text.
	gtk_text_buffer_copy_clipboard(buffer, clipboard);
}

/**
 * Menu item callback for pasting the text in the editor.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_editor_paste(GtkWidget *widget, gpointer data) {
	GtkClipboard *clipboard;
	GtkTextBuffer *buffer;

	// Get the text buffer and the default clipboard.
	buffer = get_page_editor_buffer();
	clipboard = gtk_clipboard_get(GDK_NONE);

	// Actually paste the text.
	gtk_text_buffer_paste_clipboard(buffer, clipboard, NULL, true);
}

/**
 * Menu item callback for selecting all the text in the editor.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_editor_select_all(GtkWidget *widget, gpointer data) {
	GtkTextBuffer *buffer;
	GtkTextIter start;
	GtkTextIter end;

	// Get the buffer and the iterators.
	buffer = get_page_editor_buffer();
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);

	// Do the selection.
	gtk_text_buffer_select_range(buffer, &start, &end);
}

/**
 * Menu item callback for showing the page viewer.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_show_page_viewer(GtkWidget *widget, gpointer data) {
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
}

/**
 * Menu item callback for showing the page editor.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_show_page_editor(GtkWidget *widget, gpointer data) {
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
}

/**
 * Menu item callback for showing the find dialog.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_show_dialog_find(GtkWidget *widget, gpointer data) {
	show_finder_dialog();
}

/**
 * Menu item callback for finding the next needle.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_editor_find_next(GtkWidget *widget, gpointer data) {
	find_next();
}

/**
 * Menu item callback for toogleing between the page viewer and editor.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_toggle_notebook_page(GtkWidget *widget, gpointer data) {
	GtkNotebook *nb = GTK_NOTEBOOK(notebook);

	if (gtk_notebook_get_current_page(nb) == 1) {
		gtk_notebook_prev_page(nb);
	} else {
		gtk_notebook_next_page(nb);
	}
}

/**
 * Menu item callback for showing the about dialog.
 *
 * @param widget Widget that fired this event.
 * @param data   Data passed by the signal connector.
 */
void on_show_about(GtkWidget *widget, gpointer data) {
	show_about_dialog();
}


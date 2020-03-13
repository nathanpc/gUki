/**
 * MainWindow.h
 * Helper class for dealing with the main window of the application.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <stdbool.h>
#include <gtk/gtk.h>

// Initialization and Destruction.
void initialize_mainwindow();
void window_destroy();

// Menu items and callbacks.
void on_menu_new_page(GtkWidget *widget, gpointer data);
void on_workspace_open(GtkWidget *widget, gpointer data);
void on_workspace_refresh(GtkWidget *widget, gpointer data);
void on_workspace_close(GtkWidget *widget, gpointer data);
void on_page_save(GtkWidget *widget, gpointer data);
void on_page_save_as(GtkWidget *widget, gpointer data);
void on_editor_cut(GtkWidget *widget, gpointer data);
void on_editor_copy(GtkWidget *widget, gpointer data);
void on_editor_paste(GtkWidget *widget, gpointer data);
void on_editor_select_all(GtkWidget *widget, gpointer data);
void on_show_page_viewer(GtkWidget *widget, gpointer data);
void on_show_page_editor(GtkWidget *widget, gpointer data);
void on_show_dialog_find(GtkWidget *widget, gpointer data);
void on_editor_find_next(GtkWidget *widget, gpointer data);
void on_toggle_notebook_page(GtkWidget *widget, gpointer data);
void on_show_about(GtkWidget *widget, gpointer data);

// Signal callbacks.
void on_window_delete();
void on_editor_buffer_changed(GtkTextBuffer *buffer, gpointer user_data);
void on_treeview_selection_changed(GtkWidget *widget, gpointer callback_data);
void on_treeview_popup_menu_click_delete(GtkWidget *widget, gpointer userdata);
gboolean on_treeview_popup_menu(GtkWidget *widget, GdkEventButton *event,
								gpointer userdata);
gboolean on_treeview_button_pressed(GtkWidget *widget, GdkEventButton *event,
									gpointer userdata);
void on_notebook_page_changed(GtkNotebook *notebook, GtkWidget *page,
							  guint page_num, gpointer user_data);

#endif /* _MAINWINDOW_H_ */

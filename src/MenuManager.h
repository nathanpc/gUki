/**
 * MenuManager.h
 * A helper module to manage the menus and toolbar of the application.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

#include <gtk/gtk.h>
#include <stdbool.h>

// Initialization.
void initialize_menu_manager(GtkWidget *parent_window);
GtkWidget* initialize_menubar();
GtkWidget* initialize_toolbar();

// State Change.
void update_workspace_state_menu();

#endif /* _MENUMANAGER_H_ */

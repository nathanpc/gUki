/**
 * FindReplace.h
 * Find and or replace dialog module.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#ifndef _FINDREPLACE_H_
#define _FINDREPLACE_H_

#include <gtk/gtk.h>

// Initialization and Destruction.
void initialize_find_replace(GtkWidget *main_window, GtkWidget *_editor);
void destroy_find_replace();

// Actually Find and/or Replace.
bool find_next();

// Display.
gint show_finder_dialog();

#endif /* _FINDREPLACE_H_ */

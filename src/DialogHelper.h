/**
 * DialogHelper.h
 * A simple dialog abstractor class.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#ifndef _DIALOGHELPER_H_
#define _DIALOGHELPER_H_

#include <gtk/gtk.h>
#include <stdbool.h>

// Initialization.
void initialize_dialogs(GtkWidget *parent_window);

// Message dialogs.
void message_dialog(GtkDialogFlags type, const gchar *title,
					const gchar *message_format, ...);
void warning_dialog(const gchar *title, const gchar *message_format, ...);
void error_dialog(const gchar *title, const gchar *message_format, ...);

// Special dialogs.
bool unsaved_changes_dialog();
void show_about_dialog();

#endif /* _DIALOGHELPER_H_ */

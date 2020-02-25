/**
 * DialogHelper.c
 * A simple dialog abstractor class.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#include <stdarg.h>
#include "DialogHelper.h"

// Private variables.
GtkWidget *window;

/**
 * Initializes the dialog helper.
 *
 * @param parent_window Parent window that will own the dialogs.
 */
void initialize_dialogs(GtkWidget *parent_window) {
	window = parent_window;
}

/**
 * Displays an error message dialog.
 *
 * @param title          Dialog title.
 * @param message_format Dialog message format string.
 * @param ...            Message string arguments.
 */
void error_dialog(const gchar *title, const gchar *message_format, ...) {
	GtkWidget *dialog;
	va_list argptr;

	// Create the new dialog.
	va_start(argptr, message_format);
	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
									GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
									title);

	// Add the message text to the dialog.
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
											 message_format, argptr);

	// Show the dialog and destroy it after closing.
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	va_end(argptr);
}

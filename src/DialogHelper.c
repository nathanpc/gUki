/**
 * DialogHelper.c
 * A simple dialog abstractor class.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <stdarg.h>
#include "DialogHelper.h"
#include "AppProperties.h"

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
 * Displays a message dialog.
 *
 * @param type           Dialog type flag.
 * @param title          Dialog title.
 * @param message_format Dialog message format string.
 * @param ...            Message string arguments.
 */
void message_dialog(GtkDialogFlags type, const gchar *title,
					const gchar *message_format, ...) {
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

/**
 * Displays an error message dialog.
 *
 * @param title          Dialog title.
 * @param message_format Dialog message format string.
 * @param ...            Message string arguments.
 */
void error_dialog(const gchar *title, const gchar *message_format, ...) {
	va_list argptr;

	va_start(argptr, message_format);
	message_dialog(GTK_MESSAGE_ERROR, title, message_format, argptr);
	va_end(argptr);
}

/**
 * Displays a warning message dialog.
 *
 * @param title          Dialog title.
 * @param message_format Dialog message format string.
 * @param ...            Message string arguments.
 */
void warning_dialog(const gchar *title, const gchar *message_format, ...) {
	va_list argptr;

	va_start(argptr, message_format);
	message_dialog(GTK_MESSAGE_WARNING, title, message_format, argptr);
	va_end(argptr);
}

/**
 * Shows an "Unsaved Changes" dialog.
 *
 * @return TRUE if the user wants to continue editing the page.
 */
bool unsaved_changes_dialog() {
	GtkWidget *dialog;
	gint res;

	// Create and setup dialog.
	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
									GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_QUESTION,
									GTK_BUTTONS_NONE,
									"Unsaved Changes");
	gtk_window_set_resizable(GTK_WINDOW(dialog), false);

	// Add the buttons.
	gtk_dialog_add_buttons(GTK_DIALOG(dialog),
						   GTK_STOCK_NO, GTK_RESPONSE_NO,
						   GTK_STOCK_YES, GTK_RESPONSE_YES,
						   NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);

	// Add the message text to the dialog.
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
											 "Do you want to continue editing "
											 "this page?");

	// Show the dialog and destroy it after closing.
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	return res == GTK_RESPONSE_YES;
}

/**
 * Shows an about dialog.
 */
void show_about_dialog() {
	GtkWidget *dialog;
	GtkAboutDialog *about;

	// Create the about dialog.
	dialog = gtk_about_dialog_new();
	about = GTK_ABOUT_DIALOG(dialog);

	gtk_about_dialog_set_name(about, APP_NAME);
	gtk_about_dialog_set_version(about, APP_VERSION);
	gtk_about_dialog_set_copyright(about, APP_COPYRIGHT);
	gtk_about_dialog_set_comments(about, APP_COMMENTS);
	gtk_about_dialog_set_website(about, APP_WEBSITE);

	// Show dialog and destroy it afterwards.
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

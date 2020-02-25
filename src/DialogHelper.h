/**
 * DialogHelper.h
 * A simple dialog abstractor class.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#ifndef _DIALOGHELPER_H_
#define _DIALOGHELPER_H_

#include <gtk/gtk.h>

// Initialization.
void initialize_dialogs(GtkWidget *parent_window);

// Message dialogs.
void error_dialog(const gchar *title, const gchar *message_format, ...);

#endif /* _DIALOGHELPER_H_ */

/**
 * MainWindow.h
 * Helper class for dealing with the main window of the application.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <stdbool.h>
#include <gtk/gtk.h>

// Initialization.
void initialize_mainwindow();

// Loading content.
bool load_article(const gint index);
bool load_template(const gint index);

#endif /* _MAINWINDOW_H_ */

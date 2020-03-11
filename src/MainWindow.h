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
void on_window_delete();

#endif /* _MAINWINDOW_H_ */

/**
 * gUki
 * A GTK+ implementation of the Uki personal wiki engine.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uki/uki.h>
#include <gtk/gtk.h>
#include "MainWindow.h"
#include "Workspace.h"

/**
 * Application's main entry point.
 *
 * @param  argc Number of command-line arguments supplied.
 * @param  argv Array of command-line arguments.
 * @return      Return code.
 */
int main(int argc, char **argv) {
	// Initialize GTK and the main window.
	gtk_init(&argc, &argv);
	initialize_mainwindow();

	// Enter the GTK main loop.
	gtk_main();
	return 0;
}

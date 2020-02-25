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

// Global variables.
const char *wiki_root = "/home/nathan/dev/projects/uki/gUki/TestUki/";
const char *wiki_page = "TestHTML";

// Private methods.

/**
 * Application's main entry point.
 *
 * @param  argc Number of command-line arguments supplied.
 * @param  argv Array of command-line arguments.
 * @return      Return code.
 */
int main(int argc, char **argv) {
	int uki_error;

	// Initialize the uki wiki.
	if ((uki_error = uki_initialize(wiki_root)) != UKI_OK) {
		fprintf(stderr, uki_error_msg(uki_error));
		uki_clean();

		return 1;
	}

	// Initialize GTK and the main window.
	gtk_init(&argc, &argv);
	initialize_mainwindow();

	// Populate with data.
	populate_workspace_treeview();

	// Enter the GTK main loop.
	gtk_main();

	// Clean up and return.
	uki_clean();
	return 0;
}

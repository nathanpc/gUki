/**
 * uki
 * A command-line implementation of the uki personal wiki engine.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uki/uki.h>
#include <gtk/gtk.h>

// Global variables.
const char *wiki_root = "../TestUki/";
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
	GtkWidget *window;

	// Initialize the uki wiki.
	if ((uki_error = uki_initialize(wiki_root)) != UKI_OK) {
		fprintf(stderr, uki_error_msg(uki_error));
		uki_clean();

		return 1;
	}

	// Do GTK.
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);

	g_signal_connect(window, "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();

	// Clean up and return.
	uki_clean();
	return 0;
}

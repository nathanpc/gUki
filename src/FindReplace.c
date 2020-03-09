/**
 * FindReplace.c
 * Find and or replace dialog module.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FindReplace.h"

// Private variables.
GtkWidget *parent;
GtkWidget *editor;
GtkWidget *entry_find;
GtkWidget *check_matchcase;
char *needle;
bool match_case;

// Private methods.
void store_find_state();
void restore_find_state();
void find_next();

/**
 * Initializes the find and replace module.
 *
 * @param main_window Main application window.
 * @param _editor     Editor that will have the contents searched for.
 */
void initialize_find_replace(GtkWidget *main_window, GtkWidget *_editor) {
	// Get the important widgets.
	parent = main_window;
	editor = _editor;
	entry_find = NULL;
	check_matchcase = NULL;

	// Initialize the state variables.
	match_case = false;
	needle = (char*)calloc(1, sizeof(char));
}

/**
 * Cleans up the mess we have created with this find and replace module.
 */
void destroy_find_replace() {
	free(needle);
}

/**
 * Displays the find dialog.
 *
 * @return GTK dialog response.
 */
gint show_finder_dialog() {
	GtkWidget *dialog;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *label_find;
	gint res;

	// Create the find dialog and get its vertical box container.
	dialog = gtk_dialog_new_with_buttons("Find", GTK_WINDOW(parent),
										 GTK_DIALOG_DESTROY_WITH_PARENT,
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										 GTK_STOCK_FIND, GTK_RESPONSE_OK,
										 NULL);
	vbox = GTK_DIALOG(dialog)->vbox;

	// Create the horizontal container for the find label and entry box.
	hbox = gtk_hbox_new(false, 10);
	gtk_container_set_border_width(GTK_CONTAINER(hbox), 1);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);

	// Create the find label.
	label_find = gtk_label_new("Find what:");
	gtk_box_pack_start(GTK_BOX(hbox), label_find, false, false, 0);

	// Create the find entry box.
	entry_find = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry_find, false, false, 0);

	// Create the match case checkbox.
	check_matchcase = gtk_check_button_new_with_label("Match case");
	gtk_box_pack_start(GTK_BOX(vbox), check_matchcase, false, false, 0);

	// Restore the saved state and shows the inside widgets.
	restore_find_state();
	gtk_widget_show_all(vbox);

	// Set some defaults.
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	gtk_entry_set_activates_default(GTK_ENTRY(entry_find), true);

	// Open dialog and clean up afterwards.
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	store_find_state();
	gtk_widget_destroy(dialog);

	// Check if we should perform a find operation.
	if (res == GTK_RESPONSE_OK)
		find_next();

	return res;
}

/**
 * Stores the find dialog state.
 */
void store_find_state() {
	GtkEntryBuffer *buffer;
	size_t len;

	// Get the entry buffer and its length.
	buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_find));
	len = gtk_entry_buffer_get_length(buffer);

	// Allocate the needle string and populate it.
	needle = (char*)realloc(needle, (len + 1) * sizeof(char));
	strcpy(needle, gtk_entry_buffer_get_text(buffer));

	// Check if the Match Case checkbox is checked.
	match_case = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_matchcase));
}

/**
 * Restores the find dialog state.
 */
void restore_find_state() {
	GtkEntryBuffer *buffer;

	// Get the entry buffer and set it to the needle value.
	buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_find));
	gtk_entry_buffer_set_text(buffer, needle, -1);

	// Check the Match Case checkbox.
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_matchcase), match_case);
}

/**
 * Performs a "Find Next" operation in the text view.
 */
void find_next() {
	//
}

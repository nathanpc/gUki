/**
 * FindReplace.c
 * Find and or replace dialog module.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "FindReplace.h"
#include "DialogHelper.h"

// Constants.
#define SEARCH_MARK "search_start"

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
	match_case = true;
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
#if GTK_MAJOR_VERSION == 2
										 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										 GTK_STOCK_FIND, GTK_RESPONSE_OK,
#else
										 "Cancel", GTK_RESPONSE_CANCEL,
										 "Find", GTK_RESPONSE_OK,
#endif
										 NULL);
#if GTK_MAJOR_VERSION == 2
	vbox = GTK_DIALOG(dialog)->vbox;
#else
	vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
#endif

	// Create the horizontal container for the find label and entry box.
#if GTK_MAJOR_VERSION == 2
	hbox = gtk_hbox_new(false, 10);
#else
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
#endif
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
	gtk_widget_set_sensitive(check_matchcase, false);

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
bool find_next() {
	GtkTextBuffer *buffer;
	GtkTextIter match_start;
	GtkTextIter match_end;
	GtkTextIter start;
	GtkTextMark *mark;
	char *search_needle;

	// Handle case-insensitive search.
	if (match_case) {
		search_needle = needle;
	} else {
		char *tmp;

		// Duplicate the needle string.
		search_needle = strdup(needle);
		tmp = search_needle;

		// Convert needle to lowercase.
		for (; *tmp != '\0'; tmp++) {
			*tmp = tolower(*tmp);
		}
	}

	// Get the text haystack buffer.
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor));

	// Get the search mark and create one if it doesn't exist.
	mark = gtk_text_buffer_get_mark(buffer, SEARCH_MARK);
	if (mark == NULL) {
		GtkTextIter iter;

		// Get cursor iterator and create the mark.
		gtk_text_buffer_get_iter_at_mark(buffer, &iter,
										 gtk_text_buffer_get_insert(buffer));
		mark = gtk_text_buffer_create_mark(buffer, SEARCH_MARK, &iter, false);
	}

	// Get iterator from mark and wrap around if we're at the end of the buffer.
	gtk_text_buffer_get_iter_at_mark(buffer, &start, mark);
	if (gtk_text_iter_is_end(&start))
		gtk_text_buffer_get_start_iter(buffer, &start);

	// Search for it.
	if (!gtk_text_iter_forward_search(&start, search_needle,
									  GTK_TEXT_SEARCH_TEXT_ONLY,
									  &match_start, &match_end, NULL)) {
		GtkTextIter iter;

		// Display the warning message.
		warning_dialog("String search failed", "Search string '%s' not found",
					   needle);

		// Wrap the search mark around.
		gtk_text_buffer_get_start_iter(buffer, &iter);
		gtk_text_buffer_move_mark_by_name(buffer, SEARCH_MARK, &iter);

		return false;
	}

	// Select the text we found and move the search mark.
	gtk_text_buffer_select_range(buffer, &match_start, &match_end);
	gtk_text_buffer_move_mark_by_name(buffer, SEARCH_MARK, &match_end);

	// Clean up.
	if (!match_case)
		free(search_needle);

	return true;
}

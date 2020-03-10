/**
 * PageManager.c
 * A simple module to manage the page loading, editing, and saving.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#include <uki/uki.h>
#include <webkit/webkit.h>
#include "PageManager.h"
#include "DialogHelper.h"

// Constants.
#define MAX_URI UKI_MAX_PATH + 11

// Private variables.
GtkWidget *editor;
GtkWidget *viewer;
ssize_t current_article_i;
ssize_t current_template_i;
char current_uri[MAX_URI];

// Private methods.
GtkWidget* initialize_page_editor();
GtkWidget* initialize_page_viewer();
bool load_file();

/**
 * Initializes the page manager.
 *
 * @param edit Page editor widget. (Created by this function)
 * @param view Page viewer widget. (Created by this function)
 */
void initialize_page_manager(GtkWidget **edit, GtkWidget **view) {
	// Create our main widgets.
	editor = initialize_page_editor();
	viewer = initialize_page_viewer();

	// Pass them back to our called function.
	*edit = editor;
	*view = viewer;

	// Initialize our state variables.
	current_article_i = -1;
	current_template_i = -1;
}

/**
 * Initializes the page editor (GtkTextView) control.
 *
 * @return The GtkTextView control.
 */
GtkWidget* initialize_page_editor() {
	GtkWidget *editor;

	// Create and setup editor.
	editor = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(editor), GTK_WRAP_WORD);

	return editor;
}

/**
 * Initializes the page viewer (WebKitGTK) widget.
 *
 * @return The WebKitGTK widget.
 */
GtkWidget* initialize_page_viewer() {
	GtkWidget *webview;

	// Initialize web viewer.
	webview = webkit_web_view_new();

	return webview;
}

/**
 * Loads an article to the page editor and viewer by its index.
 *
 * @param  index Article index.
 * @return       TRUE if the operation was successful.
 */
bool load_article(const gint index) {
	uki_article_t article;

	// Get the article.
	article = uki_article((size_t)index);
	if (article.name == NULL) {
		error_dialog("Unable to Find Article", "Article with index %d not "
					 "found.", index);
		return false;
	}

	// Set the state.
	current_article_i = (ssize_t)index;
	current_template_i = -1;

	// Load file contents.
	return load_file();
}

/**
 * Loads a template to the page editor and viewer by its index.
 *
 * @param  index Article index.
 * @return       TRUE if the operation was successful.
 */
bool load_template(const gint index) {
	uki_template_t template;

	// Get the template.
	template = uki_template((size_t)index);
	if (template.name == NULL) {
		error_dialog("Unable to Find Template", "Template with index %d not "
					 "found.", index);
		return false;
	}

	// Set the state.
	current_article_i = -1;
	current_template_i = (ssize_t)index;

	// Load file contents.
	return load_file();
}

/**
 * Saves the current opened page to its file.
 *
 * @return TRUE if the operation was successful.
 */
bool save_current_page() {
	GtkTextBuffer *buffer;
	GtkTextIter start;
	GtkTextIter end;
	uki_error uki_err;
	char *contents;
	char fpath[UKI_MAX_PATH];
	GError *g_err = NULL;

	// Check if we haven't opened anything yet.
	if ((current_article_i < 0) && (current_template_i < 0)) {
		error_dialog("Page Saving Failed",
					 "No article or template opened to be saved.");
		return false;
	}

	// Get page editor buffer and its contents.
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	contents = gtk_text_buffer_get_text(buffer, &start, &end, false);

	// Check if we have an article or tmeplate opened and get the file path.
	if (is_article_opened()) {
		// Get the article
		uki_article_t article = uki_article(current_article_i);

		// Get its path.
		if ((uki_err = uki_article_fpath(fpath, article)) != UKI_OK) {
			error_dialog("Error While Getting Article Path",
						 uki_error_msg(uki_err));
			return false;
		}
	} else {
		// Get the template.
		uki_template_t template = uki_template(current_template_i);

		// Get its path.
		if ((uki_err = uki_template_fpath(fpath, template)) != UKI_OK) {
			error_dialog("Error While Getting Template Path",
						 uki_error_msg(uki_err));
			return false;
		}
	}

	// Set file contents.
	if (!g_file_set_contents(fpath, contents, -1, NULL)) {
		// TODO: Use GError to get an error message.
		error_dialog("Page Saving Error", "Failed to save to the file '%s'.",
					 fpath);
		g_error_free(g_err);

		return false;
	}

	// Clean up.
	g_free(contents);
	return true;
}

/**
 * Reloads the contents of the page viewer based on the contents of the editor.
 */
void refresh_page_viewer() {
	GtkTextBuffer *buffer;
	GtkTextIter start;
	GtkTextIter end;
	char *contents;

	// Check if we haven't opened anything yet.
	if ((current_article_i < 0) && (current_template_i < 0))
		return;

	// Get page editor buffer and its contents.
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	contents = gtk_text_buffer_get_text(buffer, &start, &end, false);

	// Load the contents into the web view.
	if (is_article_opened()) {
		// Get the article
		uki_article_t article = uki_article(current_article_i);

		// Render the article.
		uki_render_article_from_text(&contents, article.deepness);
		webkit_web_view_load_string(WEBKIT_WEB_VIEW(viewer), contents, NULL,
									NULL, current_uri);
	} else {
		// Get the template.
		uki_template_t template = uki_template(current_template_i);

		// Render the template.
		uki_render_template_from_text(&contents, template.deepness);
		webkit_web_view_load_string(WEBKIT_WEB_VIEW(viewer), contents, NULL,
									NULL, current_uri);
	}

	// Free resources.
	g_free(contents);
}

/**
 * Loads the contents of a file to the page editor and viewer.
 *
 * @return TRUE if the operation was successful.
 */
bool load_file() {
	GtkTextBuffer *buffer;
	char *contents;
	uki_error uki_err;
	char fpath[UKI_MAX_PATH];
	uki_article_t article;
	uki_template_t template;
	GError *g_err = NULL;

	if (is_article_opened()) {
		// Get the article
		article = uki_article(current_article_i);

		// Get the file path.
		if ((uki_err = uki_article_fpath(fpath, article)) != UKI_OK) {
			error_dialog("Path Error", "Unable to find path for article '%s'.",
						 article.name);
			return false;
		}
	} else {
		// Get the template.
		template = uki_template(current_template_i);

		// Get the file path.
		if ((uki_err = uki_template_fpath(fpath, template)) != UKI_OK) {
			error_dialog("Path Error", "Unable to find path for template '%s'.",
						 template.name);
			return false;
		}
	}

	// Read contents.
	if (!g_file_get_contents(fpath, &contents, NULL, &g_err)) {
		// TODO: Use GError to get an error message.
		error_dialog("Article Reading Error", "Failed to read the file '%s'.",
					 fpath);
		g_error_free(g_err);

		return false;
	}

	// Get page editor buffer and set its contents.
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor));
	gtk_text_buffer_set_text(buffer, contents, -1);

	// Load the file into the web view.
	snprintf(current_uri, MAX_URI, "file://%s", fpath);
	refresh_page_viewer();

	// Free resources.
	g_free(contents);
	return true;
}

/**
 * Clears the page editor and viewer widgets.
 */
void clear_page_contents() {
	GtkTextBuffer *buffer;
	char *contents = "\n";

	// Get page editor buffer and set its contents.
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor));
	gtk_text_buffer_set_text(buffer, contents, -1);

	// Load the blank page..
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(viewer), "about:blank");
}

/**
 * Gets the page editor text buffer.
 *
 * @return The text buffer.
 */
GtkTextBuffer* get_page_editor_buffer() {
	return gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor));
}

/**
 * Create a new article.
 *
 * @param  fpath File path to the new article to be created.
 * @return       New article index.
 */
size_t new_article(const char *fpath) {
	uki_article_t article;
	size_t index;

	// Create the article and get its index.
	article = uki_add_article(fpath);
	index = uki_articles_available() - 1;

	// Set the state.
	current_article_i = (ssize_t)index;
	current_template_i = -1;

	return index;
}

/**
 * Create a new template.
 *
 * @param  fpath File path to the new template to be created.
 * @return       New template index.
 */
size_t new_template(const char *fpath) {
	uki_template_t template;
	size_t index;

	// Create the template and get its index.
	template = uki_add_template(fpath);
	index = uki_templates_available() - 1;

	// Set the state.
	current_article_i = -1;
	current_template_i = (ssize_t)index;

	return index;
}

/**
 * Checks if an article is currently opened.
 *
 * @return TRUE if an article is opened.
 */
bool is_article_opened() {
	return current_article_i >= 0;
}

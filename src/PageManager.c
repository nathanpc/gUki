/**
 * PageManager.c
 * A simple module to manage the page loading, editing, and saving.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#include "PageManager.h"
#include <uki/uki.h>
#include <webkit/webkit.h>
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
	if (current_article_i > 0) {
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
	GError *g_err;
	char *contents;
	uki_error uki_err;
	char fpath[UKI_MAX_PATH];
	uki_article_t article;
	uki_template_t template;

	if (current_article_i > 0) {
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

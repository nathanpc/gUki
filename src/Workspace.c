/**
 * Workspace.c
 * A class to manage the Uki workspace and its associated controls.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include <uki/uki.h>
#include "Workspace.h"
#include "DialogHelper.h"
#include "PageManager.h"

// Private variables.
GtkWidget *treeview;
char root_path[UKI_MAX_PATH];
bool workspace_opened;

// Private methods.
void treeview_clear();
void workspace_populate_articles(GtkTreeStore *store);
void workspace_populate_templates(GtkTreeStore *store);

/**
 * Initializes the workspace.
 *
 * @param tview TreeView widget.
 */
void initialize_workspace(GtkWidget *tview) {
	treeview = tview;
	workspace_opened = false;
}

/**
 * Opens up a Uki workspace.
 *
 * @param  wiki_root Path to the root of a Uki wiki.
 * @return           TRUE if the operation was successful.
 */
bool open_workspace(const char *wiki_root) {
	uki_error err;

	// Initialize the uki wiki.
	if ((err = uki_initialize(wiki_root)) != UKI_OK) {
		error_dialog("Error While Initializing Workspace", uki_error_msg(err));
		close_workspace();

		workspace_opened = false;
		return false;
	}

	// Populate the tree view and store the wiki root.
	populate_workspace_treeview();
	if (root_path != wiki_root)
		strcpy(root_path, wiki_root);

	// Set the opened flag and return.
	workspace_opened = true;
	return true;
}

/**
 * Closes the workspace.
 */
void close_workspace() {
	// Clear the tree view and page editor and viewer.
	treeview_clear();
	clear_page_contents();

	// Clean up our Uki mess if there was something to clean up.
	if (workspace_opened) {
		uki_clean();
	}

	workspace_opened = false;
}

/**
 * Reloads the workspace.
 */
void reload_workspace() {
	close_workspace();
	open_workspace(root_path);
}

/**
 * Populates the workspace tree view.
 */
void populate_workspace_treeview() {
	GtkTreeStore *store;
	GtkTreeModel *model;

	// Create tree view store.
	store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_INT,
							   G_TYPE_CHAR);

	// Populate the tree view.
	workspace_populate_articles(store);
	workspace_populate_templates(store);

	// Set the tree model.
	model = GTK_TREE_MODEL(store);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
	g_object_unref(model);

	// Expand the whole tree view.
	gtk_tree_view_expand_all(GTK_TREE_VIEW(treeview));
}

/**
 * Populates the tree view with Uki articles.
 *
 * @param store Tree view tree store.
 */
void workspace_populate_articles(GtkTreeStore *store) {
	GtkTreeIter root;
	GtkTreeIter child;
	GtkTreeIter folder;
	GtkTreeIter *parent;
	char last_parent[UKI_MAX_PATH];
	last_parent[0] = '\0';

	// Create articles root node.
	gtk_tree_store_append(store, &root, NULL);
	gtk_tree_store_set(store, &root, COL_NAME, "Articles", COL_INDEX, -1,
					   COL_TYPE, ROW_TYPE_TITLE, -1);

	// Go through articles.
	parent = &root;
	for (size_t i = 0; i < uki_articles_available(); i++) {
		uki_article_t article = uki_article(i);

		// Check if we have a parent.
		if (article.parent != NULL) {
			// Check if we should add a new folder.
			if (strcmp(last_parent, article.parent) != 0) {
				// Set the new last parent.
				strcpy(last_parent, article.parent);

				// Currently we only support a single deepness level.
				parent = &root;

				// Create the folder.
				gtk_tree_store_append(store, &folder, parent);
				gtk_tree_store_set(store, &folder, COL_NAME, article.parent,
								   COL_INDEX, -1, COL_TYPE, ROW_TYPE_FOLDER, -1);

				// Set the new folder as the parent.
				parent = &folder;
			}
		} else {
			// Back to the root.
			parent = &root;
		}

		// Append as a child of articles.
		gtk_tree_store_append(store, &child, parent);
		gtk_tree_store_set(store, &child, COL_NAME, article.name, COL_INDEX, i,
						   COL_TYPE, ROW_TYPE_ARTICLE, -1);
	}
}

/**
 * Populates the tree view with Uki templates.
 *
 * @param store Tree view tree store.
 */
void workspace_populate_templates(GtkTreeStore *store) {
	GtkTreeIter root;
	GtkTreeIter child;

	// Create templates root node.
	gtk_tree_store_append(store, &root, NULL);
	gtk_tree_store_set(store, &root, COL_NAME, "Templates", COL_INDEX, -1,
					   COL_TYPE, ROW_TYPE_TITLE, -1);

	// Go through articles.
	for (size_t i = 0; i < uki_templates_available(); i++) {
		uki_template_t template = uki_template(i);

		// Append as a child of articles.
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child, COL_NAME, template.name, COL_INDEX, i,
						   COL_TYPE, ROW_TYPE_TEMPLATE, -1);
	}
}

/**
 * Clears the whole workspace treeview.
 */
void treeview_clear() {
	GtkTreeModel *model;
	GtkTreeStore *store;

	// Get the tree view model.
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	if (model == NULL)
		return;

	// Get tree view store and clear it.
	store = GTK_TREE_STORE(model);
	gtk_tree_store_clear(store);
}

/**
 * Is the workspace currently opened?
 *
 * @return TRUE if the workspace is opened.
 */
bool is_workspace_opened() {
	return workspace_opened;
}

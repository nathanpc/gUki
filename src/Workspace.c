/**
 * Workspace.c
 * A class to manage the Uki workspace and its associated controls.
 *
 * @author: Nathan Campos <nathan@innoveworkshop.com>
 */

#include <uki/uki.h>
#include "Workspace.h"

// Private variables.
GtkWidget *treeview;

// Private methods.
void workspace_populate_articles(GtkTreeStore *store);
void workspace_populate_templates(GtkTreeStore *store);

/**
 * Initializes the workspace.
 *
 * @param tview TreeView widget.
 */
void initialize_workspace(GtkWidget *tview) {
	treeview = tview;
}

/**
 * Populates the workspace tree view.
 */
void populate_workspace_treeview() {
	GtkTreeStore *store;
	GtkTreeModel *model;

	// Create tree view store.
	store = gtk_tree_store_new(1, G_TYPE_STRING);

	// Populate the tree view.
	workspace_populate_articles(store);
	workspace_populate_templates(store);

	// Set the tree model.
	model = GTK_TREE_MODEL(store);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
	g_object_unref(model);
}

/**
 * Populates the tree view with Uki articles.
 *
 * @param store Tree view tree store.
 */
void workspace_populate_articles(GtkTreeStore *store) {
	GtkTreeIter root;
	GtkTreeIter child;

	// Create articles root node.
	gtk_tree_store_append(store, &root, NULL);
	gtk_tree_store_set(store, &root, 0, "Articles", -1);

	// Go through articles.
	for (size_t i = 0; i < uki_articles_available(); i++) {
		uki_article_t article = uki_article(i);

		// Append as a child of articles.
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child, 0, article.name, -1);
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
	gtk_tree_store_set(store, &root, 0, "Templates", -1);

	// Go through articles.
	for (size_t i = 0; i < uki_templates_available(); i++) {
		uki_template_t template = uki_template(i);

		// Append as a child of articles.
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child, 0, template.name, -1);
	}
}

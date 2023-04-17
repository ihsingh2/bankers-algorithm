#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <gtk/gtk.h>

int m;
int n;

GtkBuilder			*builder;
GtkWidget			*window;
GtkSpinButton			*spin1;
GtkSpinButton			*spin2;
GtkViewport			*view1;
GtkViewport			*view2;
GtkViewport			*view3;
GtkViewport			*view4;
GtkGrid				*grid1;
GtkGrid				*grid2;
GtkGrid				*grid3;
GtkGrid				*grid4;
GtkWidget			*label3;
GtkWidget			*label4;
GtkWidget			*label5;
GtkWidget			*label6;
GtkWidget			*reqbtn;
GtkWidget			*seqbtn;
GtkWidget			*label7;
GtkWidget			*alloc_entry[5][10];
GtkWidget			*max_entry[5][10];
GtkWidget			*avail_entry[5];
GtkWidget			*req_entry[6];

void on_param_set(GtkWidget *widget,gpointer data) {
	gtk_label_set_text(GTK_LABEL(label7), "");
	gtk_widget_set_visible(label3, true);
	gtk_widget_set_visible(label4, true);
	gtk_widget_set_visible(label5, true);
	gtk_widget_set_visible(label6, true);
	gtk_widget_set_visible(seqbtn, true);
	gtk_widget_set_visible(reqbtn, true);

	m = gtk_spin_button_get_value(spin1);
	n = gtk_spin_button_get_value(spin2);

	grid1 = GTK_GRID(gtk_grid_new());
	gtk_grid_set_row_spacing(grid1, 5);
	gtk_grid_set_column_spacing(grid1, 5);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			alloc_entry[i][j] = gtk_entry_new();
			gtk_widget_set_hexpand(alloc_entry[i][j], true);
			gtk_entry_set_alignment(GTK_ENTRY(alloc_entry[i][j]), 0.5);
			gtk_entry_set_input_purpose(GTK_ENTRY(alloc_entry[i][j]), GTK_INPUT_PURPOSE_NUMBER);
			gtk_grid_attach(grid1, alloc_entry[i][j], j, i, 1, 1);
		}
	}

	grid2 = GTK_GRID(gtk_grid_new());
	gtk_grid_set_row_spacing(grid2, 5);
	gtk_grid_set_column_spacing(grid2, 5);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			max_entry[i][j] = gtk_entry_new();
			gtk_widget_set_hexpand(max_entry[i][j], true);
			gtk_entry_set_alignment(GTK_ENTRY(max_entry[i][j]), 0.5);
			gtk_entry_set_input_purpose(GTK_ENTRY(max_entry[i][j]), GTK_INPUT_PURPOSE_NUMBER);
			gtk_grid_attach(grid2, max_entry[i][j], j, i, 1, 1);
		}
	}

	grid3 = GTK_GRID(gtk_grid_new());
	gtk_grid_set_column_spacing(grid3, 5);
	for (int i = 0; i < m; i++) {
		avail_entry[i] = gtk_entry_new();
		gtk_widget_set_hexpand(avail_entry[i], true);
		gtk_entry_set_alignment(GTK_ENTRY(avail_entry[i]), 0.5);
		gtk_entry_set_input_purpose(GTK_ENTRY(avail_entry[i]), GTK_INPUT_PURPOSE_NUMBER);
		gtk_grid_attach(grid3, avail_entry[i], i, 0, 1, 1);
	}

	grid4 = GTK_GRID(gtk_grid_new());
	gtk_grid_set_column_spacing(grid4, 5);
	for (int i = 0; i <= m; i++) {
		req_entry[i] = gtk_entry_new();
		gtk_entry_set_alignment(GTK_ENTRY(req_entry[i]), 0.5);
		gtk_entry_set_input_purpose(GTK_ENTRY(req_entry[i]), GTK_INPUT_PURPOSE_NUMBER);
		gtk_grid_attach(grid4, req_entry[i], i, 0, 1, 1);
	}

	gtk_viewport_set_child(view1, GTK_WIDGET(grid1));
	gtk_viewport_set_child(view2, GTK_WIDGET(grid2));
	gtk_viewport_set_child(view3, GTK_WIDGET(grid3));
	gtk_viewport_set_child(view4, GTK_WIDGET(grid4));
}

void on_seq_press(GtkWidget *widget,gpointer data) {
	gtk_label_set_text(GTK_LABEL(label7), "");

	int alloc[n][m], max[n][m], need[n][m], avail[m], seq[n], f[n], l = 0, flag = 0;

	for (int i = 0; i < n; i++) {
		f[i] = 0;
		for (int j = 0; j < m; j++) {
			alloc[i][j] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(alloc_entry[i][j]))));
			max[i][j] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(max_entry[i][j]))));
			need[i][j] = max[i][j] - alloc[i][j];
			if (alloc[i][j] < 0 || max[i][j] < 0 || need[i][j] < 0) {
				i = n;
				j = m;
				flag = 1;
			}
		}
	}

	for (int i = 0; i < m && !flag; i++) {
		avail[i] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(avail_entry[i]))));
		if (avail[i] < 0)
			flag = 1;
	}

	if (flag) {
		gtk_label_set_text(GTK_LABEL(label7), "Invalid input.");
		return;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (f[j] == 0) {
				flag = 0;
				for (int k = 0; k < m; k++) {
					if (need[j][k] > avail[k]) {
						flag = 1;
						break;
					}
				}
 				if (!flag) {
					seq[l++] = j;
					for (int q = 0; q < m; q++)
						avail[q] += alloc[j][q];
					f[j] = 1;
				}
			}
		}
	}

	char *str = malloc(3*n*sizeof(char));
	*str = 0;
	for (int i = 0; i < n; i++) {
		if (f[i] == 0) {
			gtk_label_set_text(GTK_LABEL(label7), "No safe sequence exists.");
			return;
		}
		char temp[n/10+4];
		sprintf(temp, "P%d ", seq[i]);
		strcat(str,temp);
	}
	gtk_label_set_text(GTK_LABEL(label7), str);
}

void on_req_press(GtkWidget *widget,gpointer data) {
	gtk_label_set_text(GTK_LABEL(label7), "");

	int alloc[n][m], max[n][m], need[n][m], avail[m], req[m+1], seq[n], f[n], l = 0, flag = 0;

	for (int i = 0; i < n; i++) {
		f[i] = 0;
		for (int j = 0; j < m; j++) {
			alloc[i][j] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(alloc_entry[i][j]))));
			max[i][j] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(max_entry[i][j]))));
			need[i][j] = max[i][j] - alloc[i][j];
			if (alloc[i][j] < 0 || max[i][j] < 0 || need[i][j] < 0) {
				i = n;
				j = m;
				flag = 1;
			}
		}
	}

	for (int i = 0; i < m && !flag; i++) {
		avail[i] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(avail_entry[i]))));
		if (avail[i] < 0)
			flag = 1;
	}

	for (int i = 0; i <= m && !flag; i++) {
		req[i] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(req_entry[i]))));
		if (req[i] < 0)
			flag = 1;
	}

	if (flag) {
		gtk_label_set_text(GTK_LABEL(label7), "Invalid input.");
		return;
	}

	char *str = malloc((n/10+75)*sizeof(char));
	sprintf(str, "P%d ", req[0]);

	for (int i = 0; i < m; i++) {
		if (req[i+1] > need[req[0]][i]) {
			strcat(str,"has exceeded its maximum claim.");
			gtk_label_set_text(GTK_LABEL(label7), str);
			return;
		}
	}

	for (int i = 0; i < m; i++) {
		if (req[i+1] > avail[i]) {
			strcat(str,"must wait for another process to release the resources.");
			gtk_label_set_text(GTK_LABEL(label7), str);
			return;
		}
	}

	for (int i = 0; i < m; i++) {
		alloc[req[0]][i] += req[i+1];
		avail[i] -= req[i+1];
		need[req[0]][i] -= req[i+1];
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (f[j] == 0) {
				flag = 0;
				for (int k = 0; k < m; k++) {
					if (need[j][k] > avail[k]) {
						flag = 1;
						break;
					}
				}
 				if (!flag) {
					seq[l++] = j;
					for (int q = 0; q < m; q++)
						avail[q] += alloc[j][q];
					f[j] = 1;
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		if (f[i] == 0) {
			gtk_label_set_text(GTK_LABEL(label7), "The request cannot be granted because of the safety protocol.");
			return;
		}
	}

	strcat(str,"can be safely allocated the requested resources.");
	gtk_label_set_text(GTK_LABEL(label7), str);
}

static void app_activate(GApplication *app, gpointer *user_data) {
	builder = gtk_builder_new_from_file("window.ui");
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
	spin1 = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin1"));
	spin2 = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin2"));
	view1 = GTK_VIEWPORT(gtk_builder_get_object(builder, "view1"));
	view2 = GTK_VIEWPORT(gtk_builder_get_object(builder, "view2"));
	view3 = GTK_VIEWPORT(gtk_builder_get_object(builder, "view3"));
	view4 = GTK_VIEWPORT(gtk_builder_get_object(builder, "view4"));
	label3 = GTK_WIDGET(gtk_builder_get_object(builder, "label3"));
	label4 = GTK_WIDGET(gtk_builder_get_object(builder, "label4"));
	label5 = GTK_WIDGET(gtk_builder_get_object(builder, "label5"));
	label6 = GTK_WIDGET(gtk_builder_get_object(builder, "label6"));
	reqbtn = GTK_WIDGET(gtk_builder_get_object(builder, "reqbtn"));
	seqbtn = GTK_WIDGET(gtk_builder_get_object(builder, "seqbtn"));
	label7 = GTK_WIDGET(gtk_builder_get_object(builder, "label7"));
	gtk_window_set_application(GTK_WINDOW (window), GTK_APPLICATION(app));
	gtk_window_present(GTK_WINDOW(window));
}

int main (int argc, char **argv) {
 	GtkApplication *app;
	int stat;
 	app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	stat = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return stat;
}

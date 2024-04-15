#include <gtk/gtk.h>
#include <fcntl.h>
#include <unistd.h>

void button_clicked(GtkWidget *widget, gpointer data)
{
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("Button clicked: %s\n", button_label);

    // Open the device file for writing
    int fd = open("/dev/virtual_keypad", O_WRONLY);
    if (fd < 0) {
        perror("Error opening device file");
        return;
    }

    // Write the button label to the device file
    ssize_t bytes_written = write(fd, button_label, strlen(button_label));
    if (bytes_written < 0) {
        perror("Error writing to device file");
    }

    // Close the device file
    close(fd);
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Virtual Keypad");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    button = gtk_button_new_with_label("Button 1");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label("Button 2");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

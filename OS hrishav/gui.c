#include <gtk/gtk.h>
#include <fcntl.h>
#include <unistd.h>

GtkWidget *entry; // Global variable to store the GtkEntry widget

void button_clicked(GtkWidget *widget, gpointer data)
{
    const gchar *message = gtk_entry_get_text(GTK_ENTRY(entry));
    g_print("Message to send: %s\n", message);

    // Open the device file for writing
    int fd = open("/dev/virtual_keypad", O_WRONLY);
    if (fd < 0)
    {
        perror("Error opening device file");
        return;
    }

    // Write the message to the device file
    ssize_t bytes_written = write(fd, message, strlen(message));
    if (bytes_written < 0)
    {
        perror("Error writing to device file");
    }

    // Close the device file
    close(fd);
}

void edit_button_clicked(GtkWidget *widget, gpointer data)
{
    // Clear the text entry field when Edit button is clicked
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *edit_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Virtual Keypad");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new(); // Create an editable text entry field
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 2, 1);

    button = gtk_button_new_with_label("Send Message");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    edit_button = gtk_button_new_with_label("Edit");
    g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), edit_button, 1, 1, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

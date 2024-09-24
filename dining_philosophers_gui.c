#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PHILOSOPHERS 10

pthread_mutex_t forks[MAX_PHILOSOPHERS];
int philosophers_state[MAX_PHILOSOPHERS]; // 0: thinking, 1: eating
pthread_t philosophers[MAX_PHILOSOPHERS];
int running = 0; // Control the simulation
int num_philosophers = 0;

GtkWidget *drawing_area;

void *philosopher(void *num) {
    int id = *(int *)num;

    while (running) {
        // Thinking
        philosophers_state[id] = 0;
        gtk_widget_queue_draw(drawing_area);
        sleep(rand() % 3);  // Thinking time

        // Pick up forks
        pthread_mutex_lock(&forks[id]);
        pthread_mutex_lock(&forks[(id + 1) % num_philosophers]);

        // Eating
        philosophers_state[id] = 1;
        gtk_widget_queue_draw(drawing_area);
        sleep(rand() % 3);  // Eating time

        // Put down forks
        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % num_philosophers]);
    }

    return NULL;
}

void start_simulation(GtkWidget *widget, gpointer entry) {
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(entry));
    num_philosophers = atoi(input);

    if (num_philosophers < 1 || num_philosophers > MAX_PHILOSOPHERS) {
        g_print("Invalid number of philosophers. Please enter a number between 1 and %d.\n", MAX_PHILOSOPHERS);
        return; // Do not proceed if the input is invalid
    }

    running = 1;

    // Initialize mutexes
    for (int i = 0; i < num_philosophers; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < num_philosophers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher, id);
    }
}

void stop_simulation(GtkWidget *widget, gpointer data) {
    running = 0;

    // Join philosopher threads
    for (int i = 0; i < num_philosophers; i++) {
        pthread_join(philosophers[i], NULL);
        pthread_mutex_destroy(&forks[i]);
    }

    gtk_widget_queue_draw(drawing_area); // Update drawing area
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);
    int radius = 20;

    for (int i = 0; i < num_philosophers; i++) {
        int x = (width / (num_philosophers + 1)) * (i + 1);
        int y = height / 2;

        // Draw stick figure
        cairo_arc(cr, x, y, radius, 0, 2 * G_PI); // head
        cairo_stroke(cr);

        // Body
        cairo_move_to(cr, x, y + radius);
        cairo_line_to(cr, x, y + radius + 40);
        cairo_stroke(cr);

        // Arms
        if (philosophers_state[i] == 1) {
            // Eating (draw arms out)
            cairo_move_to(cr, x, y + radius + 10);
            cairo_line_to(cr, x - 20, y + radius + 10);
            cairo_move_to(cr, x, y + radius + 10);
            cairo_line_to(cr, x + 20, y + radius + 10);
        } else {
            // Thinking (draw arms down)
            cairo_move_to(cr, x, y + radius + 10);
            cairo_line_to(cr, x - 20, y + radius + 20);
            cairo_move_to(cr, x, y + radius + 10);
            cairo_line_to(cr, x + 20, y + radius + 20);
        }
        cairo_stroke(cr);

        // Draw philosopher number
        cairo_move_to(cr, x - 10, y + radius + 50);
        cairo_show_text(cr, g_strdup_printf("Philosopher %d", i + 1)); // Display 1 to n

        // Draw state (Thinking or Eating)
        cairo_move_to(cr, x - 20, y + radius + 70);
        if (philosophers_state[i] == 1) {
            cairo_show_text(cr, "Eating");
        } else {
            cairo_show_text(cr, "Thinking");
        }
        cairo_stroke(cr);
    }

    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *stop_button = gtk_button_new_with_label("Stop");
    GtkWidget *entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), start_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), stop_button, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 400, 300);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_simulation), entry);
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_simulation), NULL);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    gtk_widget_show_all(window);
    gtk_window_set_title(GTK_WINDOW(window), "Dining Philosophers");

    gtk_main();

    return 0;
}


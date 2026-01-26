#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "c_ergas.h"


pthread_mutex_t tel_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cook_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oven_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t deliverer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cook_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t oven_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t deliverer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t tel_cond = PTHREAD_COND_INITIALIZER;

int tel_available = Ntel;
int cook_available = Ncook;
int oven_available = Noven;
int deliverer_available = Ndeliverer;
int serviceTimes[Ntel] = {0};
int coolingTimes[Ntel] = {0};
int successful_orders = 0;
int failed_orders = 0;
int total_sales = 0;



void create_order(CUSTOMER* customer) {
    // Ορισμός περιόδου παραγγελίας μεταξύ Torderlow και Torderhigh
    int order_period = rand() % (Torderhigh - Torderlow + 1) + Torderlow;
    sleep(order_period); // Προσομοίωση περιόδου παραγγελίας

    // Ορισμός πλήθους πίτσας προς παραγγελία μεταξύ Norderlow και Norderhigh
    customer->numberOfPizzas = rand() % (Norderhigh - Norderlow + 1) + Norderlow;

    // Προσομοίωση διαδικασίας παραγγελίας
    printf("Customer %d ordered %d pizzas.\n", customer->ID, customer->numberOfPizzas);
}

void release_resources() {
    pthread_mutex_lock(&tel_mutex);
    tel_available++;
    pthread_mutex_unlock(&tel_mutex);

    pthread_mutex_lock(&cook_mutex);
    cook_available++;
    pthread_cond_signal(&cook_cond);
    pthread_mutex_unlock(&cook_mutex);

    pthread_mutex_lock(&oven_mutex);
    oven_available++;
    pthread_cond_signal(&oven_cond);
    pthread_mutex_unlock(&oven_mutex);

    pthread_mutex_lock(&deliverer_mutex);
    deliverer_available++;
    pthread_cond_signal(&deliverer_cond);
    pthread_mutex_unlock(&deliverer_mutex);
}

bool acquire_resources() {
    bool resources_acquired = false;

    pthread_mutex_lock(&tel_mutex);
    if (tel_available > 0) {
        tel_available--;
        resources_acquired = true;
    }
    pthread_mutex_unlock(&tel_mutex);

    if (!resources_acquired) {
        return false;
    }

    pthread_mutex_lock(&cook_mutex);
    while (cook_available <= 0) {
        pthread_cond_wait(&cook_cond, &cook_mutex);
    }
    cook_available--;
    pthread_mutex_unlock(&cook_mutex);

    pthread_mutex_lock(&oven_mutex);
    while (oven_available <= 0) {
        pthread_cond_wait(&oven_cond, &oven_mutex);
    }
    oven_available--;
    pthread_mutex_unlock(&oven_mutex);

    pthread_mutex_lock(&deliverer_mutex);
    while (deliverer_available <= 0) {
        pthread_cond_wait(&deliverer_cond, &deliverer_mutex);
    }
    deliverer_available--;
    pthread_mutex_unlock(&deliverer_mutex);

    return true;
}


// Ενημέρωση στατιστικών και χρόνων εξυπηρέτησης
void update_statistics(int numberOfPizzas, int id) {
    if (numberOfPizzas > 0) {
        // Επιτυχημένη παραγγελία
        successful_orders++;
        total_sales += numberOfPizzas;

        // Υπολογισμός χρόνου εξυπηρέτησης
        int serviceTime = Tprep + Tbake + Tpack;
        serviceTimes[id] = serviceTime;

        // Υπολογισμός χρόνου κρυώματος
        int coolingTime = Tdellow + rand() % (Tdelhigh - Tdellow + 1);
        coolingTimes[id] = coolingTime;
    } else {
        // Αποτυχημένη παραγγελία
        failed_orders++;
    }
}
// Εύρεση μέσου χρόνου εξυπηρέτησης
double mean_service_time() {
    int sum = 0;
    for (int i = 0; i < Ntel; i++) {
        sum += serviceTimes[i];
    }
    return (double)sum / successful_orders;
}

// Εύρεση μέγιστου χρόνου εξυπηρέτησης
int max_service_time() {
    int max = serviceTimes[0];
    for (int i = 1; i < Ntel; i++) {
        if (serviceTimes[i] > max) {
            max = serviceTimes[i];
        }
    }
    return max;
}

// Εύρεση μέσου χρόνου κρυώματος
double mean_cooling_time() {
    int sum = 0;
    for (int i = 0; i < Ntel; i++) {
        sum += coolingTimes[i];
    }
    return (double)sum / successful_orders;
}

// Εύρεση μέγιστου χρόνου κρυώματος
int max_cooling_time() {
    int max = coolingTimes[0];
    for (int i = 1; i < Ntel; i++) {
        if (coolingTimes[i] > max) {
            max = coolingTimes[i];
        }
    }
    return max;
}
void print_statistics() {
    printf("Total sales: %d euros\n", total_sales);
    printf("Successful orders: %d\n", successful_orders);
    printf("Failed orders: %d\n", failed_orders);
    printf("Mean service time: %.2f minutes\n", mean_service_time());
    printf("Max service time: %d minutes\n", max_service_time());
    printf("Mean cooling time: %.2f minutes\n", mean_cooling_time());
    printf("Max cooling time: %d minutes\n", max_cooling_time());
}
void order_failed() {
    failed_orders++;
}

void* order(void* arg) {
    CUSTOMER* customer = (CUSTOMER*)arg;
    int id = customer->ID;
    int numberOfPizzas = rand() % (Norderhigh - Norderlow + 1) + Norderlow;
    int orderTime = rand() % (Torderhigh - Torderlow + 1) + Torderlow;

    // Ελέγχουμε τη διαθεσιμότητα της γραμμής τηλεφώνου
    if (!acquire_resources()) {
        printf("Order %d: Phone line busy. Order failed.\n", id);
        order_failed();
        pthread_exit(NULL);
    }

    printf("Order %d: Phone line acquired.\n", id);

    // Επεξεργαζόμαστε την παραγγελία
    printf("Order %d: Processing order...\n", id);
    sleep(orderTime);
    printf("Order %d: Order processed.\n", id);

    // Απελευθερώνουμε τους πόρους
    release_resources();

    // Ενημερώνουμε τα στατιστικά
    update_statistics(numberOfPizzas,id);

    // Τυπώνουμε τα αντίστοιχα μηνύματα
    if (numberOfPizzas > 0) {
        printf("Order %d: Order with %d pizzas prepared in %d minutes.\n", id, numberOfPizzas, Tprep + Tbake + Tpack);
        printf("Order %d: Order with %d pizzas delivered in %d minutes.\n", id, numberOfPizzas, Tdellow + rand() % (Tdelhigh - Tdellow + 1));
    } else {
        printf("Order %d: Order failed.\n", id);
    }

    // Τερματίζουμε το νήμα
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    
    // Δημιουργία πίνακα πελατών
    CUSTOMER customers[Ntel];
    pthread_t threads[Ntel];
    
    // Δημιουργία νημάτων για κάθε πελάτη
    for (int i = 0; i < Ntel; i++) {
        customers[i].ID = i + 1;
        pthread_create(&threads[i], NULL, order, &customers[i]);
    }
    
    // Αναμονή τερματισμού όλων των νημάτων
    for (int i = 0; i < Ntel; i++) {
        pthread_join(threads[i], NULL);
    }
    print_statistics();
    
    return 0;
}
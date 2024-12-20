#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int arrival_time;
    int checkout_duration;
    int wait_time;
} Customer;

typedef struct {
    Customer *customers;
    int size;
    int capacity;
} Queue;

void initQueue(Queue *queue, int capacity) {
    queue->customers = (Customer *)malloc(sizeof(Customer) * capacity);
    queue->size = 0;
    queue->capacity = capacity;
}

void enqueue(Queue *queue, Customer customer) {
    if (queue->size < queue->capacity) {
        queue->customers[queue->size++] = customer;
    }
}

Customer dequeue(Queue *queue) {
    Customer customer = queue->customers[0];
    for (int i = 1; i < queue->size; ++i) {
        queue->customers[i - 1] = queue->customers[i];
    }
    queue->size--;
    return customer;
}

int isQueueEmpty(Queue *queue) {
    return queue->size == 0;
}

# Seeding the RNG 
int pgrand(int x, int y) {
    int i;
    long long total = 0; 

    for (i = 0; i < 6; i++)
        total += rand(); 
    total *= (y - x);
    total /= (((long long) RAND_MAX) * 6);
    return x + total; 
}

int main() {
    int simulation_length, num_express_lines, num_regular_lines;
    int avg_time_between_customers, percent_express;
    int avg_express_checkout_time, min_express_checkout_time;
    int avg_regular_checkout_time, min_regular_checkout_time;
    int total_express_customers = 0, total_regular_customers = 0;
    int total_express_wait_time = 0, total_regular_wait_time = 0;

    srand(time(NULL)); 

    printf("Enter simulation length (in seconds): ");
    scanf("%d", &simulation_length);
    printf("Enter number of open express lines: ");
    scanf("%d", &num_express_lines);
    printf("Enter number of open regular lines: ");
    scanf("%d", &num_regular_lines);
    printf("Enter average time between customers entering checkout line (in seconds): ");
    scanf("%d", &avg_time_between_customers);
    printf("Enter percentage of customers that qualify as express: ");
    scanf("%d", &percent_express);
    printf("Enter average time needed for an express customer to checkout (in seconds): ");
    scanf("%d", &avg_express_checkout_time);
    printf("Enter minimum time needed for an express customer to checkout (in seconds): ");
    scanf("%d", &min_express_checkout_time);
    printf("Enter average time needed for a regular customer to checkout (in seconds): ");
    scanf("%d", &avg_regular_checkout_time);
    printf("Enter minimum time needed for a regular customer to checkout (in seconds): ");
    scanf("%d", &min_regular_checkout_time);

    Queue express_lines[num_express_lines];
    Queue regular_lines[num_regular_lines];
    for (int i = 0; i < num_express_lines; ++i) {
        initQueue(&express_lines[i], simulation_length);
    }
    for (int i = 0; i < num_regular_lines; ++i) {
        initQueue(&regular_lines[i], simulation_length);
    }

    int current_time = 0; # initializes current_time 
    while (current_time < simulation_length) {
        int next_customer_time = pgrand(0, avg_time_between_customers * 2);
        current_time += next_customer_time;

        if (current_time >= simulation_length) break;

        int is_express = (rand() % 100) < percent_express;
        int checkout_time = is_express ? pgrand(min_express_checkout_time, avg_express_checkout_time * 2 - min_express_checkout_time)
                                       : pgrand(min_regular_checkout_time, avg_regular_checkout_time * 2 - min_regular_checkout_time);
        
        Customer new_customer = {current_time, checkout_time, 0};

        Queue *target_line = NULL;
        int min_size = simulation_length;

        if (is_express) {
            for (int i = 0; i < num_express_lines; ++i) {
                if (express_lines[i].size < min_size) {
                    target_line = &express_lines[i];
                    min_size = express_lines[i].size;
                }
            }
            if (!target_line) {
                for (int i = 0; i < num_regular_lines; ++i) {
                    if (regular_lines[i].size < min_size) {
                        target_line = &regular_lines[i];
                        min_size = regular_lines[i].size;
                    }
                }
            }
        } else {
            for (int i = 0; i < num_regular_lines; ++i) {
                if (regular_lines[i].size < min_size) {
                    target_line = &regular_lines[i];
                    min_size = regular_lines[i].size;
                }
            }
        }

        if (target_line) {
            if (!isQueueEmpty(target_line)) {
                new_customer.wait_time = target_line->customers[target_line->size - 1].arrival_time +
                                         target_line->customers[target_line->size - 1].checkout_duration - current_time;
                new_customer.wait_time = new_customer.wait_time < 0 ? 0 : new_customer.wait_time;
            }
            enqueue(target_line, new_customer);
            if (is_express) {
                total_express_customers++;
                total_express_wait_time += new_customer.wait_time;
            } else {
                total_regular_customers++;
                total_regular_wait_time += new_customer.wait_time;
            }
        }
    }

    printf("Number of express customers who checked out: %d\n", total_express_customers);
    printf("Average wait time for express customers: %f seconds\n", total_express_customers > 0 ? (double)total_express_wait_time / total_express_customers : 0);
    printf("Number of regular customers who checked out: %d\n", total_regular_customers);
    printf("Average wait time for regular customers: %f seconds\n", total_regular_customers > 0 ? (double)total_regular_wait_time / total_regular_customers : 0);

    for (int i = 0; i < num_express_lines; ++i) {
        free(express_lines[i].customers);
    }
    for (int i = 0; i < num_regular_lines; ++i) {
        free(regular_lines[i].customers);
    }

    return 0;
}

Code Provided:

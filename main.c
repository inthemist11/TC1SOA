#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

sem_t semaphore;
int state = 0;
pthread_t last;
int bridge_size_secs = 20;


// A normal C function that is executed as a thread
// when its name is specified in pthread_create()

double ran_expo(double lambda, double current_time)
{
    double u;
    u = rand() / (RAND_MAX + 1.0);
    double exp = -lambda * log(u);
    return exp + current_time;
}

//Función que ejecuta el thread
void *thread_simulation(void *direction)
{
    printf("Car created with direction = %d\n", direction);

    pthread_t carId = pthread_self();

    printf("Car id = %d\n", carId);

    if (state != direction)
    {
        printf("Estoy apunto de esperar\n");
        sem_wait(&semaphore);
        printf("Ya esperé\n");
    }
    else
    {
        if (state == 0)
        {
             printf("Cambiar dirección\n");
            state = direction;
        }
        printf("Puse que soy el ultimo\n");
        last = carId;
        printf("Voy a dormir\n");
        sleep(bridge_size_secs);
        printf("Desperte\n");
        if (last == carId)
        {
            printf("Voy a poner neutro\n");
            state = 0;
        }
         printf("Soltar el semaforo\n");
        sem_post(&semaphore);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    sem_init(&semaphore, 0, 1);

    int right_cars = 5;
    int left_cars = 5;
    int left_lambda = 5;
    int right_lambda = 5;
    

    //Parameters parsing
    if (argc > 2)
    {
        right_cars = atoi(argv[1]);
        left_cars = atoi(argv[2]);
    }
    if (argc > 4)
    {
        right_lambda = atoi(argv[3]);
        left_lambda = atoi(argv[4]);
    }

    printf("Parameters set at: East cars: %d, West cars: %d, East lambda: %d, West lambda: %d\n", right_cars, left_cars, right_lambda, left_lambda);

    srand(time(NULL));
    pthread_t thread_id;
    printf("Before Thread\n");

    double depart_time_left[left_cars];
    double depart_time_right[right_cars];
    int cars_direction[left_cars + right_cars];
    double depart_time_total[left_cars + right_cars];

    //Define times for left side
    double current_time = 0;
    for (int i = 0; i < left_cars; i++)
    {

        double depart_time = ran_expo(left_lambda, current_time);
        depart_time_left[i] = depart_time;
        current_time = depart_time;
    }

    printf("===================Left Times ============== \n");
    for (int i = 0; i < left_cars; i++)
    {
        printf("Time = %f \n", depart_time_left[i]);
    }

    //Define times for right side
    current_time = 0;
    for (int i = 0; i < right_cars; i++)
    {

        double depart_time = ran_expo(right_lambda, current_time);
        depart_time_right[i] = depart_time;
        current_time = depart_time;
    }

    printf("===================Right Times ============== \n");
    for (int i = 0; i < right_cars; i++)
    {
        printf("Time = %f \n", depart_time_right[i]);
    }

    //Sort both lists
    int aux_left = 0;
    int aux_right = 0;
    aux_right++;

    int i = 0, j = 0, k = 0;
    while (i < left_cars && j < right_cars)
    {
        cars_direction[k] = depart_time_left[i] < depart_time_right[j] ? -1 : 1;
        depart_time_total[k++] = depart_time_left[i] < depart_time_right[j] ? depart_time_left[i++] : depart_time_right[j++];
    }

    while (i < left_cars)
    {
        cars_direction[k] = -1;
        depart_time_total[k++] = depart_time_left[i++];
    }

    while (j < right_cars)
    {
        cars_direction[k] = 1;
        depart_time_total[k++] = depart_time_right[j++];
    }

    printf("===================Total Times and Direction ==============");
    for (int i = 0; i < left_cars + right_cars; i++)
    {
        printf("Time = %f Direction = %d \n", depart_time_total[i], cars_direction[i]);
    }
    i = 0;
    while (i < left_cars + right_cars)
    {
        if (i != 0)
        {
            sleep(depart_time_total[i] - depart_time_total[i - 1]);
        }
        pthread_t pthread;
        pthread_create(&pthread, NULL, thread_simulation, (void *)cars_direction[i]);
        i++;
    }

    exit(0);
}

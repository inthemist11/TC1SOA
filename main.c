#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

sem_t semaphoreLeft;
sem_t semaphoreRigth;
int state = 0;
pthread_t last;
int bridge_size_secs = 5;

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()

double ran_expo(double lambda, double current_time)
{
    double u;
    u = rand() / (RAND_MAX + 1.0);
    double exp = -lambda * log(u);
    return exp + current_time;
}

void *thread_simulation(void *direction)
{
    printf("\nNUEVO CARRO ID: %d (%d) \n", pthread_self(), direction);

    if (state != direction && state != 0) //Entra si hay carros en dirección contraria pasando el puente
    {
        printf("%d\t(%d)\t%d\t%d\tVoy a hacer fila\n", pthread_self(), direction, state, last);
        direction == 1 ? sem_wait(&semaphoreRigth) : sem_wait(&semaphoreLeft); //Pide el semáforo
        direction == 1 ? sem_post(&semaphoreRigth) : sem_post(&semaphoreLeft); //Suelta el semáforo
        printf("%d\t(%d)\t%d\t%d\tYa hice la fila y voy yo\n", pthread_self(), direction, state, last);
    }
    if (state == 0) //Entra si el puente está vacío
    {
        printf("%d\t(%d)\t%d\t%d\tEl puente está vacío y voy a cambiar el estado\n", pthread_self(), direction, state, last);
        //Pide los dos semaforos
        sem_wait(&semaphoreRigth); 
        sem_wait(&semaphoreLeft);
        direction == 1 ? sem_post(&semaphoreRigth) : sem_post(&semaphoreLeft);//Suelta el semáforo de la dirección de él
        state = direction;
    }
    else //Entra si el puente está de nuestro lado
    {
        printf("%d\t(%d)\t%d\t%d\tEn el puente hay carros de mi misma dirección\n", pthread_self(), direction, state, last);
        direction == 1 ? sem_wait(&semaphoreRigth) : sem_wait(&semaphoreLeft); //Pide el semáforo
        direction == 1 ? sem_post(&semaphoreRigth) : sem_post(&semaphoreLeft); //Suelta el semáforo
    }
    last = pthread_self(); //Actualiza el último
    printf("%d\t(%d)\t%d\t%d\tA4. Voy a pasar el puente\n", pthread_self(), direction, state, last);
    sleep(bridge_size_secs); //Pasa el puente
    printf("%d\t(%d)\t%d\t%d\tA5. Ya pasé el puente\n", pthread_self(), direction, state, last);
    if (last == pthread_self()) //Valida si soy el último
    {
        state = 0;                                                             //Devuelve el puente
        printf("%d\t(%d)\t%d\t%d\tA6. El puente quedó libre\n", pthread_self(), direction, state, last);
        direction == 1 ? sem_post(&semaphoreLeft) : sem_post(&semaphoreRigth); //Devuleve el semáforo del contrario
    }

    return NULL;
}

int main(int argc, char **argv)
{
    sem_init(&semaphoreRigth, 0, 1);
    sem_init(&semaphoreLeft, 0, 1);

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
    pthread_t pthread;
    while (i < left_cars + right_cars)
    {
        if (i != 0)
        {
            sleep(depart_time_total[i] - depart_time_total[i - 1]);
        }
        pthread_create(&pthread, NULL, thread_simulation, (void *)cars_direction[i]);
        i++;
    }

    i = 0;
    while (i < left_cars + right_cars)
    {
        pthread_join(pthread, NULL);
        i++;
    }
    pthread_exit(0);
}

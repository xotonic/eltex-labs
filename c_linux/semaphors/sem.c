
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

// Кол-во запускаемых процессов
#define PNUM 10
// Макс. число процессов, пропускаемых семафором
#define SEMVAL 1
// Размер массива чисел
#define N 1000000
// Крутой макрос для отладки IPC вызовов
#define CHECK(x) if ((size_t)x == -1) \
{\
	printf("Error in %s %d, code: %d\n%s\n", __FUNCTION__, __LINE__,errno, strerror(errno)); exit(1);\
}

// Для semctl
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} arg;

/*
	Находит среднее по масссиву a размерности n
*/
float avg(float* a, int n) {
	float sum = 0.0f;
	for (int i = 0; i < n; i++) {
		sum += a[i];
	}
	float avg = sum / (float)n;
	return avg;
}

// Для use()
struct sembuf lock = {0, -1, 0};
struct sembuf unlock = {0, 1, 0};

/*
	Выполняется дочерними процессами
	semid - ID семафора
	shmid - ID общей памяти
	
	1) Процесс уменьшает число семафора на 1
	2) Вычисляет мат. ожидание по массиву
	3) Удваивает числа для след процесса
	4) Увеличивает число семафора на 1
*/
void use(int semid, int shmid)
{
	CHECK(semop(semid, &lock, 1));
	int val = semctl(semid, 0, GETVAL, 0);
	CHECK(val);
	printf("pid %d: Locked - %d\n", getpid(), val);

	float* ptr = shmat(shmid, 0, 0);
	CHECK((size_t)ptr);

	float favg = avg(ptr, N);
	// multipling numbers for next process
	for (int i = 0; i < N; i++)
	{
		ptr[i] *= 2.0f;
	}

	CHECK(semop(semid, &unlock, 1));
	val = semctl(semid, 0, GETVAL, 0);
	CHECK(val);

	printf("pid %d: Unlocked - %d, avg = %f\n", getpid(), val, favg);

}

int main()
{
	key_t semkey = 1488;

	// Создаем семафор
	arg.val = SEMVAL;
	printf("Creating new semaphore: max %d processes\n", arg.val);
	int semid = semget(semkey, 1, 0666 | IPC_CREAT);
	CHECK(semid);
	CHECK(semctl(semid, 0, SETVAL, arg));

	// Создаем разделяему память
	int n = N;
	int shmid = shmget(semkey, n * sizeof(float), 0666 | IPC_CREAT | IPC_EXCL);
	CHECK(shmid);

	// Получаем указатель для заполнения
	float* ptr = (float*)shmat(shmid, 0, 0);
	CHECK(ptr);

	// Заполняем рандомными числами
	printf("Generating %d random numbers...\n", n);
	for (int i = 0; i < n; i++)
	{
		ptr[i] = rand() % 1000;
	}
	// Запускаем процессы
	int pnum = PNUM;
	printf("Starting %d processes...\n", pnum);
	printf("Press ENTER to free memory\n");
	for (int i = 0; i < pnum; i++)
	{
		pid_t pid =  fork();
		if (pid == 0)
		{
			use(semid, shmid);
			exit(1);
		}
	}

	getchar();

	// Детачим память, затем удаляем ее и семафор
	printf("Removing shared memory and semaphore...\n");
	CHECK(shmdt((void*)ptr));
	CHECK(shmctl(shmid, IPC_RMID, 0));
	CHECK(semctl(semid, 0, IPC_RMID, 0));

	printf("Done\n");

	return 0;
}
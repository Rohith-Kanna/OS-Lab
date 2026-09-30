#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include <sys/shm.h>
#include<stdbool.h>
#define SHMSZ 1024
#define MAX_LIMIT 1024
int main() {
int shmid;
key_t key;
key = 3415;
char *shm, *s, *k;
if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
perror("shmget");
exit(1);
}
if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
perror("shmat");
exit(1);
}
s = shm;
printf("\n %d is the Write process ID that is writing into Shared Memory...\n", getpid());
while (*s != '\0')
s++;
char x = *s; // The data here should be null
printf("\n The Data here should be null ( coz this is the end of shm) : %c \n", x);
char str[MAX_LIMIT];
int choice;
do {
printf("\n Enter the String to be entered into the Shared Memory: ");
fgets(str, MAX_LIMIT, stdin); // Will accept input till ENTER key is hit...
printf("\nAre you sure the string you want to enter is: %s",str);
printf("\nIf YES -> 1 | NO -> 0: ");
scanf("%d", &choice);
getchar(); // Consume the newline character left by scanf
} while (choice != 1);
printf("The String that will be entered into the SHM is: %s\n",str);
// Copy the string into shared memory
for (int i = 0; i < MAX_LIMIT; i++) {
*s++ = str[i]; // After the last memory location, s will point to NULL
if (str[i] == '\0')
break;
}
sleep(5); // Simulate some processing time
printf("\nThe Write process for %d Process is complete!!!\n",getpid());
printf("Contents of the shared memory after writing: ");
for (k = shm; *k != '\0'; k++) {
printf("%c", *k);
}
putchar('\n');
return 0;
}

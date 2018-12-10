#include <stdio.h>   // printf, perror, fgets
#include <sys/ipc.h> // ftok, shmctl+
#include <sys/shm.h> // shmget, shmat, shmctl+
#include <stdlib.h>  // calloc, free
#include <string.h>  // strcmp, strerror
#include <errno.h>   // ERRNO

int main() {
    key_t key = ftok( "./tester.txt", 'R' );
    int shmid = shmget( key, 201, 0644);
    char *data = (char *) calloc ( 201, sizeof(char) ); // 201*1 bytes

    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    if (shmid == -1) {
        printf("Creating segment...\n");
        shmid = shmget( key, 201, 0644 | IPC_CREAT );
    }
    else
        data = shmat( shmid, (void *)0, 0 );

    // changing data
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    else {
        printf("Shared contents: %s\n", data);

        char *input = calloc( 1, sizeof(char) );
        printf("Change data in segment? (y/n) ");
        scanf("%s", input); // get letter and \0 from stdin, bind to input

        if ( !strcmp(input,"y") ) { // if =0 thus =y
            printf("New data: ");
            scanf("%s", data); // get 200-byte string and \0 from stdin
        }

        free(input); // no use for input anymore...
    }

    // deleting segment
    char *input2 = calloc( 1, sizeof(char) );
    printf("Delete memory segment? (y/n) ");
    scanf("%s", input2); // get letter and \0 from stdin, bind to input2

    if ( !strcmp(input2,"y") ) { // if =0 thus =y
        int success = shmctl( shmid, IPC_RMID, NULL );
        if ( success == -1 )  // not 0
            printf( "%s\n", strerror(errno) );
        else
            printf( "Memory segment deleted\n" );
    }
    
    free(input2); // no use for input2 anymore...
    
    return 0;
}

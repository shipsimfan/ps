#include <los/session.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    sid_t sid;

    if (argc == 1)
        sid = get_session_id();
    else if (argc == 2)
        sid = atoi(argv[1]);
    else {
        fprintf(stderr, "Invalid number of arguments\n");
        fprintf(stderr, "USAGE: %s [SESSION]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    isize num_processes = get_session_processes(sid, NULL, 0);
    if (num_processes < 0) {
        fprintf(stderr, "Failed to get processes: %s\n",
                strerror(num_processes));
        exit(EXIT_FAILURE);
    }

    pid_t* processes = (pid_t*)malloc(sizeof(pid_t) * num_processes);
    num_processes = get_session_processes(sid, processes, num_processes);
    if (num_processes < 0) {
        fprintf(stderr, "Failed to get processes: %s\n",
                strerror(num_processes));
        exit(EXIT_FAILURE);
    }

    process_t info;
    info.working_directory = (char*)malloc(sizeof(char) * 1024);
    info.working_directory_len = 1024;
    info.name = (char*)malloc(sizeof(char) * 256);
    info.name_len = 256;

    for (isize i = 0; i < num_processes; i++) {
        isize ret = get_process_info(sid, processes[i], &info);
        if (ret < 0)
            continue;

        printf("%ld %s (%lds)\n", processes[i], info.name, info.time / 1000);
        printf("    (Threads: %ld) (Files: %ld) (Directories: %ld)\n",
               info.num_threads, info.num_files, info.num_directories);
        printf("    %s\n", info.working_directory);
    }

    free(processes);
    free(info.name);
    free(info.working_directory);
}
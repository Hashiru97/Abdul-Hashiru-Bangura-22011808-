#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_JOBS 100

int preemptive = 0;

typedef struct {
  int arrival_time;
  int burst_time;  
  int priority;
} Job;

int read_jobs(FILE *input_file, Job jobs[]) {
  int num_jobs = 0;
  while (fscanf(input_file, "%d %d %d", &jobs[num_jobs].arrival_time, &jobs[num_jobs].burst_time, &jobs[num_jobs].priority) == 3) {
    num_jobs++;
  }
  return num_jobs;
}

void fcfs(Job jobs[], int num_jobs) {
   FILE *output_file = freopen("output.txt", "a", stdout);
  int current_time = 0;
  int wait_time = 0;  
  int turnaround_time = 0;  
  for (int i = 0; i < num_jobs; i++) {
    printf("Processing job %d\n", i + 1);
    if (current_time < jobs[i].arrival_time) {
      current_time = jobs[i].arrival_time;
    }
    wait_time += current_time - jobs[i].arrival_time;
    turnaround_time += current_time - jobs[i].arrival_time + jobs[i].burst_time;
    current_time += jobs[i].burst_time;

  }
  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
  printf("Average turnaround time: %f\n", (double)turnaround_time / num_jobs);
  fclose(output_file);
}

void sjf(Job jobs[], int num_jobs) {
 
    int current_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int preemptive_choice;

    printf("Do you want to use preemptive SJF? (1 for Yes, 0 for No): ");
    scanf("%d", &preemptive_choice);
      FILE *output_file = freopen("output.txt", "a", stdout);
    while (1) {
        int shortest_job = -1;
        for (int j = 0; j < num_jobs; j++) {
            if (jobs[j].arrival_time <= current_time && jobs[j].burst_time > 0) {
                if (shortest_job == -1 || jobs[j].burst_time < jobs[shortest_job].burst_time) {
                  if(shortest_job == -1 || jobs[j].burst_time)
                    shortest_job = j;
                }
              
            }
            
        }

        if (shortest_job == -1) {
            break;
        }

        printf("Processing job %d\n", shortest_job + 1);
        total_wait_time += current_time - jobs[shortest_job].arrival_time;
        total_turnaround_time += current_time - jobs[shortest_job].arrival_time + jobs[shortest_job].burst_time;

        if (preemptive_choice) {
           
            current_time++;
            jobs[shortest_job].burst_time--;
        } else {
            
            current_time += jobs[shortest_job].burst_time;
            jobs[shortest_job].burst_time = 0;
        }
    }

    printf("Total Average wait time: %f\n", (double)total_wait_time / num_jobs);
    printf("Total Average turnaround time: %f\n", (double)total_turnaround_time / num_jobs);

     fclose(output_file);
}

void priority(Job jobs[], int num_jobs) {
  FILE *output_file = freopen("output.txt", "a", stdout);
  int current_time = 0;
 int wait_time = 0;
    int turnaround_time = 0;
  while (1) {
    
    int highest_priority_job = -1;
    for (int j = 0; j < num_jobs; j++)  {
        if (jobs[j].burst_time > 0) {
            if (highest_priority_job == -1 ||
                jobs[j].priority < jobs[highest_priority_job].priority ||
                (jobs[j].priority == jobs[highest_priority_job].priority && jobs[j].arrival_time < jobs[highest_priority_job].arrival_time)) {
                highest_priority_job = j;
            }
        }
    }
    if (highest_priority_job == -1) {
        break;
    }
    
    printf("Processing job %d\n", highest_priority_job + 1);
    wait_time += current_time - jobs[highest_priority_job].arrival_time;
    turnaround_time += current_time - jobs[highest_priority_job].arrival_time + jobs[highest_priority_job].burst_time;
    current_time += jobs[highest_priority_job].burst_time;
    jobs[highest_priority_job].burst_time = 0;
    if (preemptive) {
      continue;
    }
     int all_jobs_completed = 1;
    for (int j = 0; j < num_jobs; j++) {
      if (jobs[j].burst_time > 0) {
        all_jobs_completed = 0;
        break;
      }
    }

    if (all_jobs_completed) {
      break;
    }
   
  }
  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
  printf("Average turnaround time: %f\n", (double)turnaround_time / num_jobs);

   fclose(output_file);
  
}

void round_robin(Job jobs[], int num_jobs, int time_slice) {
  FILE *output_file = freopen("output.txt", "a", stdout);
    int current_time = 0;
    int wait_time = 0;
    int turnaround_time = 0;
    int jobs_completed = 0;

    while (jobs_completed < num_jobs) {
        for (int j = 0; j < num_jobs; j++) {
            if (jobs[j].arrival_time <= current_time && jobs[j].burst_time > 0) {
                printf("Processing job %d\n", j + 1);
                wait_time += current_time - jobs[j].arrival_time;

                if (jobs[j].burst_time <= time_slice) {
                    turnaround_time += current_time - jobs[j].arrival_time + jobs[j].burst_time;
                    current_time += jobs[j].burst_time;
                    jobs[j].burst_time = 0;
                    jobs_completed++;
                } else {
                    turnaround_time += current_time - jobs[j].arrival_time + time_slice;
                    current_time += time_slice;
                    jobs[j].burst_time -= time_slice;
                }
            }
        }
    }

    printf("Average wait time: %f\n", (double)wait_time / num_jobs);
    printf("Average turnaround time: %f\n", (double)turnaround_time / num_jobs);
     fclose(output_file);
}

int choice;
void print_menu(choice) {
    printf("\nCPU Scheduler Simulator\n");
    printf("1) Scheduling Method (None)\n");
    printf("2) Preemptive Mode (Off)\n");
    printf("3) Show Result\n");
    printf("4) End Program\n");
    printf("Option > ");
}

int main(int argc, char **argv) {
  int opt;
  char *input_file_name = "C:\\Users\\abdul\\OneDrive\\Desktop\\School work\\Projects\\project OS\\Scheduling\\input.txt";
  char *output_file_name = "C:\\Users\\abdul\\OneDrive\\Desktop\\School work\\Projects\\project OS\\Scheduling\\output.txt";
   int algorithm_choice;
    int scheduling_method = 0; // 0: None, 1: FCFS, 2: SJF, 3: Priority, 4: Round Robin
     int preemptive_mode = 0; // 0: Off, 1: On
  while ((opt = getopt(argc, argv, "f:o:p")) != -1) {
    switch (opt) {
      case 'f':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 'p':
        preemptive = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-f input_file] [-o output_file] [-p]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (input_file_name == NULL) {
    fprintf(stderr, "Error: No input file specified.\n");
    exit(EXIT_FAILURE);
  }

  FILE *input_file = fopen(input_file_name, "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Could not open input file %s.\n", input_file_name);
    exit(EXIT_FAILURE);
  }

  Job jobs[MAX_JOBS];
  int num_jobs = read_jobs(input_file, jobs);
  fclose(input_file);

  printf("\t\t\t\t\t\t CPU Scheduler Simulator:\n");
    printf("1. First-Come First-Serve\n");
    printf("2. Shortest Job First\n");
    printf("3. Priority Scheduling\n");
    printf("4. Round Robin\n");

    printf("Enter the number corresponding to your choice: ");
    scanf("%d", &algorithm_choice);

  FILE *output_file = NULL;
  if (output_file_name != NULL) {
    output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
      fprintf(stderr, "Error: Could not open output file %s.\n", output_file_name);
      exit(EXIT_FAILURE);
    }
  }

  switch (algorithm_choice) {
        case 1:
            printf("\nFirst-Come First-Serve:\n");
            fcfs(jobs, num_jobs);
            break;
        case 2:
            printf("\nShortest Job First:\n");
            sjf(jobs, num_jobs);
            break;
        case 3:
            printf("\nPriority Scheduling:\n");
            priority(jobs, num_jobs);
            break;
        case 4:
            printf("\nRound Robin (time slice = 2):\n");
            round_robin(jobs, num_jobs, 2);
            break;
        default:
            fprintf(stderr, "Invalid choice. Exiting...\n");
            exit(EXIT_FAILURE);
    }

  if (output_file != NULL) {
    fclose(output_file);
  }

  return 0;
}

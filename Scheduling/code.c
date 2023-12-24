#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_JOBS 100

typedef struct {
  int arrival_time;
  int burst_time;
  int original_burst_time;
  int priority;
} Job;

int preemptive = 0;

void reset_jobs(Job jobs[], int num_jobs) {
  for (int i = 0; i < num_jobs; i++) {
    jobs[i].burst_time = jobs[i].original_burst_time;
  }
}

int read_jobs(FILE *input_file, Job jobs[]) {
  int num_jobs = 0;
  while (fscanf(input_file, "%d %d %d",  &jobs[num_jobs].burst_time, &jobs[num_jobs].arrival_time, &jobs[num_jobs].priority) == 3) {
    jobs[num_jobs].original_burst_time = jobs[num_jobs].burst_time;
    num_jobs++;
  }
  return num_jobs;
}

void fcfs(Job jobs[], int num_jobs, FILE *output_file) {
  int current_time = 0;
  int wait_time = 0;
  
    printf("Scheduling Method: First Come First Served\n");
    fprintf(output_file, "Scheduling Method: First Come First Served\n");
    printf("Process Waiting Times:\n");
    fprintf(output_file, "Process Waiting Times:\n");
  for (int i = 0; i < num_jobs; i++) {
    if (current_time < jobs[i].arrival_time) {
      current_time = jobs[i].arrival_time;
    }
    wait_time += current_time - jobs[i].arrival_time;

        printf("P%d: %d ms\n", i + 1, wait_time);
        fprintf(output_file, "P%d: %d ms\n", i + 1, wait_time);

       
    current_time += jobs[i].burst_time;
  }
   double average_wait_time = (double)wait_time / num_jobs;
    printf("Average Waiting Time: %f ms\n", average_wait_time);
    fprintf(output_file, "Average Waiting Time: %f ms\n", average_wait_time);
 
}

void sjf(Job jobs[], int num_jobs, FILE *output_file) {
  int current_time = 0;
  int total_wait_time = 0;
  int jobs_finished = 0; 
  
  int preemptive_choice;

  printf("Do you want to use preemptive SJF? (1 for Yes, 0 for No): ");
  scanf("%d", &preemptive_choice);

  printf("Scheduling Method: Shortest Job First – %s\n", preemptive_choice ? "Preemptive" : "Non-Preemptive");
  fprintf(output_file, "Scheduling Method: Shortest Job First – %s\n", preemptive_choice ? "Preemptive" : "Non-Preemptive");
  printf("Process Waiting Times:\n");
  fprintf(output_file, "Process Waiting Times:\n");

  
  while (jobs_finished < num_jobs) {
    int shortest_job = -1;
    for (int j = 0; j < num_jobs; j++) {
      if (jobs[j].arrival_time <= current_time && jobs[j].burst_time > 0) {
        if (shortest_job == -1 || jobs[j].burst_time < jobs[shortest_job].burst_time) {
          shortest_job = j;
        }
      }
    }

    if (shortest_job == -1) {
     break;
    } 

     printf("P%d: %d ms\n", shortest_job + 1, total_wait_time);
    fprintf(output_file, "P%d: %d ms\n", shortest_job + 1, total_wait_time);
    total_wait_time += current_time - jobs[shortest_job].arrival_time;
    

     int current_arrival_time = 0;
     if (preemptive_choice) {
      jobs[shortest_job].burst_time--;
      current_time++;
      if(current_time == jobs[shortest_job].arrival_time && current_time > 0)
    {
      current_arrival_time += jobs[shortest_job].burst_time;
      current_time == current_arrival_time;
     if (jobs[shortest_job].burst_time == 0) {
        // If the job is completed, update turnaround time
        jobs_finished++;
        current_time += jobs[shortest_job].arrival_time - current_time;
      }
    }
    } else {
      current_time += jobs[shortest_job].burst_time;
      jobs[shortest_job].burst_time = 0;
      jobs_finished++;
    }
     
  }
    double average_wait_time = (double)total_wait_time / num_jobs;
    printf("Average Waiting Time: %f ms\n", average_wait_time);
    fprintf(output_file, "Average Waiting Time: %f ms\n", average_wait_time);
}

void priority(Job jobs[], int num_jobs, FILE *output_file) {
  int current_time = 0;
  int wait_time = 0;
 
  int preemptive_choice;

  printf("Do you want to use preemptive Priority? (1 for Yes, 0 for No): ");
  scanf("%d", &preemptive_choice);

  printf("Scheduling Method: Priority – %s\n", preemptive_choice ? "Preemptive" : "Non-Preemptive");
  fprintf(output_file, "Scheduling Method: Priority – %s\n", preemptive_choice ? "Preemptive" : "Non-Preemptive");
  printf("Process Waiting Times:\n");
  fprintf(output_file, "Process Waiting Times:\n");

  while (1) {
    int highest_priority_job = -1;
    for (int j = 0; j < num_jobs; j++) {
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
    
    printf("P%d: %d ms\n", highest_priority_job + 1, wait_time);
    fprintf(output_file, "P%d: %d ms\n", highest_priority_job + 1, wait_time);
    wait_time += current_time - jobs[highest_priority_job].arrival_time;

    if (preemptive_choice) {
      current_time++;
      jobs[highest_priority_job].burst_time--;

      if (jobs[highest_priority_job].burst_time == 0) {
        // If the job is completed, update turnaround time
        current_time += jobs[highest_priority_job].arrival_time - current_time;
      }
    } else {
      current_time += jobs[highest_priority_job].burst_time;
      jobs[highest_priority_job].burst_time = 0;
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

  double average_wait_time = (double)wait_time / num_jobs;
  printf("Average wait time: %f ms\n", average_wait_time);
  fprintf(output_file, "Average wait time: %f ms\n", average_wait_time);
}

void round_robin(Job jobs[], int num_jobs, int time_slice, FILE *output_file) {
  int current_time = 0;
  int wait_time = 0;
 
  int jobs_completed = 0;

  while (jobs_completed < num_jobs) {
    for (int j = 0; j < num_jobs; j++) {
      if (jobs[j].arrival_time <= current_time && jobs[j].burst_time > 0) {
        printf("Processing job %d\n", j + 1);
        fprintf(output_file, "Processing job %d\n", j + 1);
        wait_time += current_time - jobs[j].arrival_time;

        if (jobs[j].burst_time <= time_slice) {
         
          current_time += jobs[j].burst_time;
          jobs[j].burst_time = 0;
          jobs_completed++;
        } else {
          
          current_time += time_slice;
          jobs[j].burst_time -= time_slice;
        }
      }
    }
  }

  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
  fprintf(output_file, "Average wait time: %f\n", (double)wait_time / num_jobs);
 
}

int main(int argc, char **argv) {
  int opt;
  char *input_file_name = "C:\\Users\\abdul\\OneDrive\\Desktop\\School work\\Projects\\project OS\\Scheduling\\input.txt";
  char *output_file_name = "C:\\Users\\abdul\\OneDrive\\Desktop\\School work\\Projects\\project OS\\Scheduling\\output.txt";
  int algorithm_choice;
  int scheduling_method = 0;  // 0: None, 1: FCFS, 2: SJF, 3: Priority, 4: Round Robin
  int preemptive_choice = 0; // 0: Off, 1: On

  FILE *output_file = NULL;

  while ((opt = getopt(argc, argv, "f:o:p")) != -1) {
    switch (opt) {
      case 'f':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 'p':
        preemptive = 1 - preemptive_choice;
        break;
      default:
        fprintf(stderr, "Usage: %s [-f input_file] [-o output_file] [-p]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
while (1) {
  FILE *input_file = fopen(input_file_name, "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Could not open input file %s.\n", input_file_name);
    exit(EXIT_FAILURE);
  }

  output_file = fopen(output_file_name, "a");
  if (output_file == NULL) {
    fprintf(stderr, "Error: Could not open output file %s.\n", output_file_name);
    fclose(input_file);
    exit(EXIT_FAILURE);
  }

  Job jobs[MAX_JOBS];
  int num_jobs = read_jobs(input_file, jobs);
  fclose(input_file);

  
    printf("\t\t\t\t\t\t CPU Scheduler Simulator:\n");
    printf("1. Scheduling Method (None)\n");
    printf("2. Preemptive Mode (Off)\n");
    printf("3. Show Result\n");
    printf("4. End Program\n");
    printf("Option > ");

    scanf("%d", &algorithm_choice);

    switch (algorithm_choice) {
      case 1:
        printf("\nChoose Scheduling Method:\n");
        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. Priority\n");
        printf("4. Round Robin\n");
        printf("0. None\n");
        printf("Enter the number corresponding to your choice: ");
        scanf("%d", &scheduling_method);
        break;
      case 2:
        printf("\nPreemptive Mode:\n");
        printf("0. Off\n");
        printf("1. On\n");
        printf("Enter the number corresponding to your choice: ");
        scanf("%d", &preemptive_choice);
        break;
      case 3:
        printf("\nShow Result:\n");
        switch (scheduling_method) {
          case 1:
            fcfs(jobs, num_jobs, output_file);
            break;
          case 2:
            sjf(jobs, num_jobs, output_file);
            break;
          case 3:
            priority(jobs, num_jobs, output_file);
            break;
          case 4:
            round_robin(jobs, num_jobs, 2, output_file);
            break;
          default:
            printf("\nNo Scheduling Method Selected.\n");
        }
        break;
      case 4:
        printf("\nEnd Program.\n");
        fclose(output_file);
        exit(EXIT_SUCCESS);
      default:
        printf("\nInvalid choice. Please enter a valid option.\n");
    }
  }

  return 0;
}
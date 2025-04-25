/*Assignment name : tsp
Expected files : *.c *.h
Allowed functions: write, sqrtf, getline, fseek, fscanf, ferror, feof, fabsf, 
memcpy, fprintf, fclose, malloc, calloc, ralloc, free, fopen, errno, stderr, 
stdin, stdout

-------------------------------------------------------
The first publication referring to this problem as the "traveling salesman
problem" is found in the 1949 RAND Corporation report by Julia Robinson,
"On the Hamiltonian game (a traveling salesman problem)."
Here is hwo she defines the problem:
"The purpose of this note is to give a method for solving a problem related
to the traveling salesman problem. It seems worthwhile to give a description
of the original problem. One formulation is to find the shortest route for a
salesman starting from Washington, visiting all the state capitals and the
returning to Washington.
More generally, to find the shortest CLOSED CURVE containing n given points in the plane."
for example with the following set of cities:
0, 0
1, 0
2, 0
0, 1
1, 1
2, 1
1, 2
2, 2
which can be presented as follows:
+ + +
+ + +
 + +

the shortest path is:
 _____
|__ | 
  |__|
  
  so you should print the length of this path that is:
  8.00
  
  Write a program that will read a set of city coordinates under the form
  '%f, %f\n' from the standard input and will print the length of the shortest
  possible path containing all these cities under the form '%.2f'.
  Your program will not be tested with more than 11 cities.
  You will find in this directory a file tsp.c containing all the boring 
  parts of this exercise and example files to help you test your program.
  hint: in order to use sqrtf, add -lm at the end of your compilation command.
  For example this should work:
  $> cat square.txt
  1, 1
  0, 1
  1, 0
  0, 0
  $> ./tsp < square.txt | cat -e
  4.00$*/


 #include <unistd.h>
 #include <math.h>
 #include <stdio.h>

  #define MAX 11
  #define INF 1e9
  
  typedef struct 
  {
      float x;
      float y;
  } City;
  
  // Global variables
  City cities[MAX];
  int num_cities = 0;
  int path[MAX];
  int best_path[MAX];
  float min_distance = INF;
  
  // Calculate Euclidean distance between two cities
  float distance(City a, City b) 
  {
      return sqrtf(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
  }
  
  // Calculate total path length
  float calculate_path_length() 
  {
      float total = 0;
      for (int i = 0; i < num_cities - 1; i++) 
      {
          total += distance(cities[path[i]], cities[path[i + 1]]);
      }
      // Add distance back to starting city
      total += distance(cities[path[num_cities - 1]], cities[path[0]]);
      return total;
  }
  
  // Swap two elements in array
  void swap(int *a, int *b) 
  {
      int temp = *a;
      *a = *b;
      *b = temp;
  }
  
  /**
 * @brief Generates all permutations of the cities using Heap's algorithm.
 *
 * This function recursively generates all permutations of the city indices
 * and calculates the total distance for each permutation. If a shorter path
 * is found, it updates the global variables `min_distance` and `best_path`.
 *
 * @param k The size of the current subset of the array to permute.
 */
  void generate_permutations(int k) 
  {
      if (k == 1) 
      {
          float current_distance = calculate_path_length();
          if (current_distance < min_distance) 
          {
              min_distance = current_distance;
              for (int i = 0; i < num_cities; i++) 
              {
                  best_path[i] = path[i];
              }
          }
          return;
      }
  
      for (int i = 0; i < k; i++) 
      {
          generate_permutations(k - 1);
          if (k % 2 == 0) 
          {
              swap(&path[i], &path[k - 1]);
          } 
          else 
          {
              swap(&path[0], &path[k - 1]);
          }
      }
  }

  /**
 * @brief Main function to solve the TSP.
 *
 * Reads city coordinates from standard input, generates all permutations of
 * the cities, and calculates the shortest path. The result is printed to
 * standard output with exactly two decimal places.
 *
 * @return 0 on successful execution.*/
  
  int main() 
  {
      // Read city coordinates from stdin
      float x, y;
      while (fscanf(stdin, "%f, %f", &x, &y) == 2 && num_cities < MAX)
      {
          cities[num_cities].x = x;
          cities[num_cities].y = y;
          path[num_cities] = num_cities;  // Initialize path array
          num_cities++;
      }
  
      if (num_cities == 0) {
          fprintf(stdout, "0.00\n");
          return 0;
      }
  
      // Generate all possible permutations and find shortest path
      generate_permutations(num_cities);
  
      // Print result with exactly 2 decimal places
      fprintf(stdout, "%.2f\n", min_distance);
  
      return 0;
  }
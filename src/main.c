#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DS18_PATH "/sys/bus/w1/devices/28-00000652e0f5/w1_slave" // ugh, dont do this.
#define pbufsize 100

void safefree(void **pp) {
  assert(pp);
  if(pp != NULL) {
    free(*pp);
    *pp = NULL;
  }
}

char *get_tmp(char *path) {
  FILE *fp;
  char buffer[pbufsize];
  size_t bytes_read;

  fp = fopen(path, "r");

  if(!fp) {
    fprintf(stderr, "Error while opening: %s\n", path);
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  while(fgets(buffer, pbufsize, fp) != NULL);

  fclose(fp);
  fp = NULL;
  size_t len = strlen(buffer) - 1;
  if(len > 0 && buffer[len] == '\n')
    buffer[len] = '\0';

  strtok(buffer, "t=");
  char *tmp = strtok(NULL, "t=");
  return strdup(tmp);
}

int main(void) {
  char *temp;
  temp = get_tmp(DS18_PATH);

  float celcius;
  celcius = strtof(temp, NULL);
  celcius = celcius / 1000;
  printf("%02.3f \n", celcius);

  safefree((void **)&temp);
  return EXIT_SUCCESS;
}

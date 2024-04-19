#include <iostream>

#include "includes/array.hpp"

void print_array(const char *name, pxd::Array<int> &arr) {
  printf("---------------------------------------------------------------------"
         "-------\n");
  printf("%s array :\n", name);
  int i = 0;
  int size = arr.get_length();

  for (i = 0; i < size; i++) {
    printf("%6d -> %6d\n", i + 1, arr[i]);
  }
  printf("---------------------------------------------------------------------"
         "-------\n");
}

#define S 10

int main() {
  int temp[S];

  for (int i = 0; i < S; i++) {
    temp[i] = i;
  }

  pxd::Array<int> arr(temp, ARRAY_SIZE(temp));

  pxd::Array<int> t(10);
  arr.copy_to(t);

  if (t == arr) {
    printf("True\n");
  } else {
    printf("False\n");
  }

  t.resize(5);

  print_array("temp", t);

  int last_value = arr[9];
  std::cout << last_value << "\n";

  return 0;
}
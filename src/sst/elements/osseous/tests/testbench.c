    // Copyright 2009-2015 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2015, Sandia Corporation
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#include "tb_header.h"
#include <memory.h>
#include <sint.h>
#include <stdio.h>
#include <stdlib.h>
#include <uint.h>
#include <utility>
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <uint.h> 
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {

  const int LENGTH = 32768;
  int start = 0;

  ariel_enable();

  printf("Allocating arrays of size %d elements.\n", LENGTH);
  double *a = (double *)mlm_malloc(sizeof(double) * LENGTH, 0);
  double *b = (double *)mlm_malloc(sizeof(double) * LENGTH, 0);
  double *fast_c = (double *)mlm_malloc(sizeof(double) * LENGTH, 0);

  UInt<1> reset;
  char* inp_ptr;
  UInt<1> *ctrl_ptr;
  UInt<32> *ctrl_pt;

  mlm_set_pool(1);

  printf("Allocation for fast_c is %llu\n", (unsigned long long int)fast_c);
  double *c = (double *)malloc(sizeof(double) * LENGTH);
  printf("Done allocating arrays.\n");

  int i;
  for (i = 0; i < LENGTH; ++i) {
    a[i] = i;
    b[i] = LENGTH - i;
    c[i] = 0;
  }

  // Issue a memory copy
  mlm_memcpy(fast_c, c, sizeof(double) * LENGTH);

  printf("Perfoming the fast_c compute loop...\n");
#pragma omp parallel for
  for (i = 0; i < LENGTH; ++i) {
    // printf("issuing a write to: %llu (fast_c)\n", ((unsigned long long int)
    // &fast_c[i]));
    fast_c[i] = 2.0 * a[i] + 1.5 * b[i];
  }

  reset = UInt<1>(1);

  size_t inp_size = 1L << 32; 
  size_t ctrl_size = sizeof(UInt<32>) * 3; 
  RTL_shmem_info *shmem = new RTL_shmem_info(inp_size, ctrl_size);

  inp_ptr = (char *)shmem->get_inp_ptr();
  ctrl_ptr = (UInt<1>*)shmem->get_ctrl_ptr();
  ctrl_pt = (UInt<32>*)ctrl_ptr;
  ctrl_pt++;

  std::ifstream in("multiply.riscv.hex");
  if (!in) {
    std::cout << "could not open " << "multiply.riscv.hex" << std::endl;
    exit(EXIT_FAILURE);
  }

  //uint64_t cnt = 0;
  std::string line;
  while (std::getline(in, line)) {
    #define parse_nibble(c) ((c) >= 'a' ? (c)-'a'+10 : (c)-'0')
    for (int i = line.length()-2, j = 0; i >= 0; i -= 2, j++) {
      inp_ptr[start + j] = (parse_nibble(line[i]) << 4) | parse_nibble(line[i+1]);
      //cnt++;
    }
    start += line.length()/2;
  }

  //printf("\nCounter is: %ld", cnt);

  Update_RTL_Params *params = new Update_RTL_Params();
  ctrl_ptr[0] = reset;
  params->storetomem(shmem);
  ctrl_ptr[0] = reset;
  params->check(shmem);
  params->storetomem(shmem);

  ariel_fence();
  start_RTL_sim(shmem);
  bool *check = (bool *)shmem->get_inp_ptr();
  printf("\nSimulation started\n");

  reset = UInt<1>(0);
  params->perform_update(false, true, true, true, true, false, false, 5);
  params->storetomem(shmem);
  params->check(shmem);
  params->storetomem(shmem);
  ariel_fence();
  update_RTL_sig(shmem);

  ctrl_ptr[0] = reset;
  ctrl_pt[0] = UInt<32>(0); 
  ctrl_pt[1] = UInt<32>(0); 
  /*ctrl_ptr[0] = reset;
  ctrl_pt[0] = UInt<32>(0); 
  ctrl_pt[1] = UInt<32>(0); */


  params->perform_update(false, true, true, true, true, true, false, 1000L);
  params->storetomem(shmem);
  params->check(shmem);
  params->storetomem(shmem);
  ariel_fence();
  update_RTL_sig(shmem);

  params->perform_update(false, true, true, true, true, true, true, 10);
  params->storetomem(shmem);
  params->storetomem(shmem);
  update_RTL_sig(shmem);

  // Now copy results back
  mlm_Tag copy_tag = mlm_memcpy(c, fast_c, sizeof(double) * LENGTH);
  mlm_waitComplete(copy_tag);

  double sum = 0;
  for (i = 0; i < LENGTH; ++i) {
    sum += c[i];
  }

  printf("Sum of arrays is: %f\n", sum);
  printf("Freeing arrays...\n");

  mlm_free(a);
  mlm_free(b);
  mlm_free(fast_c);
  free(c);

  delete shmem;
  delete params;

  printf("Done.\n");

  return 0;
}

/* SGCT-Based-Fault-Tolerant-3D-SFI Code source file.
   Copyright (c) 2015, Md Mohsin Ali. All rights reserved.
   Licensed under the terms of the BSD License as described in the LICENSE_FT_CODE file.
   This comment must be retained in any redistributions of this source file.
*/

/* 
 * File       : FailureRecovery.h
 * Description: Contains functions for reconstructing faulty communicator by
 *              performing in-order failed process replacement, and other 
 *              supporting functions to achieve this
 * Author     : Mohsin Ali
 * Created    : August 2013
 * Updated    : May 2014, June 2014, September 2014
 */

#ifndef FAILURERECOVERY_INCLUDED
#define FAILURERECOVERY_INCLUDED

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "mpi.h"
#include <unistd.h> //getopt(), gethostname()
#ifndef NON_FT
#include "mpi-ext.h"
#endif
#ifdef _OPENMP
#include <omp.h>
#endif

// Function prototype
MPI_Comm communicatorReconstruct(MPI_Comm myCommWorld, int childFlag, int * listFails, int * numFails,
                int * numNodeFails, int sumPrevNumNodeFails, int argc, char ** argv, int verbosity);

int numProcsFails(MPI_Comm comm);

void mpiErrorHandler(MPI_Comm * comm, int *errorCode, ...);

void repairComm(MPI_Comm * broken, MPI_Comm * repaired, int iteration, int * listFails, int * numFails,
                int * numNodeFails, int sumPrevNumNodeFails, int argc, char ** argv, int verbosity);

int rankIsNotOnFailedList(int rank, int * failedList, int numFails);

char * getHostToLaunch(int hostfileLineIndex);

int getHostfileLastLineIndex(void);

int getSlots(void);

#endif /*FAILURERECOVERY_INCLUDED*/


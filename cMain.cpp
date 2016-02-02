/* SGCT-Based-Fault-Tolerant-3D-SFI Code source file.
 * Copyright (c) 2015, Md Mohsin Ali. All rights reserved.
 * Licensed under the terms of the BSD License as described in the LICENSE_FT_CODE file.
 * This comment must be retained in any redistributions of this source file.
 */

/*
 * cMain.cpp
 *
 * Created on: 16 November 2014
 * Author    : Mohsin Ali
 * Updated   :
 *
 */

// Header files
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<iostream>

using namespace std;

// Function prototypes to wrapper routines to be called from C
#ifdef __cplusplus
extern "C"{
#endif
   void c_runLBM(char *, MPI_Fint *);
   void c_get_LBM_field(int * ndims, double * fi_1d, double * fi_2d, double * fi_3d, \
            int * x_start, int * y_start, int * z_start, int * x_width, int * y_width, int * z_width);
#ifdef __cplusplus
}
#endif

// Global variable declaration
double * field_2d = NULL;
int my_x_width, my_y_width;

// MAIN FUNCTION
int main(int argc, char** argv)
{

    MPI_Init(&argc, &argv);

    // Variable declaration
    MPI_Comm c_comm_world = MPI_COMM_WORLD;
    MPI_Comm c_split_comm_world;
    MPI_Fint fortran_comm_world;
    int rank, size;
    int grid_id = 0, grid_rank = 0;
    char parInDir[128] = "param_grid_", gridIdStr[128] = " ";
    double wtime_start = 0.0; 

    // A typecast is needed
    // "http://www.roguewave.com/DesktopModules/Bring2mind/DMX/Download.aspx?entryid=751&command=core_download&PortalId=0&TabId=607"

    MPI_Comm_rank(c_comm_world, &rank);
    MPI_Comm_size(c_comm_world, &size);

    // Split c_comm_world into one or two or multiple worlds
    grid_id = rank / 8/*0*/;
    grid_rank = rank % 8/*rank*/;

    sprintf(gridIdStr,"%d", grid_id);
    strcat(parInDir, gridIdStr);

    MPI_Comm_split(c_comm_world, grid_id, grid_rank, &c_split_comm_world);

    // Convert C communicator handle to Fortran handle
    fortran_comm_world = MPI_Comm_c2f(c_split_comm_world);

    if(rank == 0){
       printf("\n*******************************************************\n");
       printf("******* Calling TaxilaLBM from C side           *******\n");
       printf("*******************************************************\n\n");
       fflush(stdout);
    }

    // Start wallclock time
    wtime_start = MPI_Wtime();
    // Only one LBM simulation
    //c_runLBM(parInDir, &fortran_comm_world);

    // Print field values received from TaxilaLBM
    if (rank == 0) {
    for (int i = 0; i < my_x_width*my_y_width; i++) {
       printf("c value = %lf\n", field_2d[i]);
    }
    }
    
    // Stop wallclock time and calculate elapsed time
    if(rank == 0){
       printf("Total wallclock time for TaxilaLBM: %0.2lf sec\n", MPI_Wtime() - wtime_start);
    }

    fflush(stdout);
 
    MPI_Finalize();
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"{
#endif
void c_get_LBM_field(int * ndims, double * fi_1d, double * fi_2d, double * fi_3d, \
            int * x_start, int * y_start, int * z_start, int * x_width, int * y_width, int * z_width) {
    my_x_width = *x_width;
    my_y_width = *y_width;
    
    field_2d = new double [my_x_width*my_y_width];

    for (int i = 0; i < my_x_width*my_y_width; i++) {
       field_2d[i] = fi_2d[i];
    }
}
#ifdef __cplusplus
}
#endif


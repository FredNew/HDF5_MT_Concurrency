#include "hdf5.h"
#include "dataset.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../src/H5Ppublic.h"

#define H5Z_FILTER_LZ4 32004

int main(int argc, char* argv[]){

	struct timespec start, end;

    int rank = 2;

    hsize_t dims[rank];
    hsize_t chunk_dims[rank];

    dims[0] = 4*1024;
    dims[1] = 4*1024;

    chunk_dims[0] = 1024; //down
    chunk_dims[1] = 1024; //right

    hsize_t dset_size = dims[0] * dims[1];
    hsize_t chunk_size = chunk_dims[0] * chunk_dims[1];

    int* dset = create_int_ds(dset_size);

    hid_t file_id, fspace_id, dset_id, dcpl_id;

    file_id = H5Fcreate("Outfile.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    
    dcpl_id = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(dcpl_id, rank, chunk_dims);
    
    const unsigned int cd_values[1] = {8*1024};

    if(H5Zfilter_avail(H5Z_FILTER_LZ4)){
        H5Pset_filter(dcpl_id, H5Z_FILTER_LZ4, H5Z_FLAG_MANDATORY, (size_t) 1, cd_values);
    }else{
        printf("Filter %d not found. Not applied.\n", H5Z_FILTER_LZ4);
    }

    H5Pset_deflate(dcpl_id, 3);

    fspace_id = H5Screate_simple(rank, dims, NULL);
    dset_id = H5Dcreate(file_id, "LZ4 parallel", H5T_NATIVE_INT, fspace_id, H5P_DEFAULT, dcpl_id, H5P_DEFAULT);

    clock_gettime(CLOCK_MONOTONIC, &start);
    H5Dwrite_filter_parallel(dset_id, H5S_ALL, dset, 4);
    clock_gettime(CLOCK_MONOTONIC, &end);

    free(dset);

    H5Dclose(dset_id);
    H5Sclose(fspace_id);
    H5Fclose(file_id);

    double elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec)/1000000000.0;
    printf("; %lf\n", elapsed); //Wall clock
    return 0;
}

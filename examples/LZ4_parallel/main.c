#include "hdf5.h"
#include "dataset.h"
#include <stdio.h>
#include <stdlib.h>

#define H5Z_FILTER_LZ4 32004

int main(int argc, char* argv[]){
    int rank = 2;

    hsize_t dims[rank];
    hsize_t chunk_dims[rank];

    dims[0] = 4*16*1024;
    dims[1] = 4*1024;

    chunk_dims[0] = 4*1024; //down
    chunk_dims[1] = 1024; //right

    hsize_t dset_size = dims[0] * dims[1];
    hsize_t chunk_size = chunk_dims[0] * chunk_dims[1];

    int* dset = create_int_ds(dset_size);

    printf("Dataset created.\n");

    hid_t file_id, fspace_id, dset_id, dcpl_id;

    file_id = H5Fcreate("Outfile.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    
    dcpl_id = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(dcpl_id, rank, chunk_dims);
    
    const unsigned int cd_values[1] = {8*1024};

    if(H5Zfilter_avail(H5Z_FILTER_LZ4)){
        printf("Filter %d available. Being applied. Block-Setting: %d.\n", H5Z_FILTER_LZ4, cd_values[0]);
        H5Pset_filter(dcpl_id, H5Z_FILTER_LZ4, H5Z_FLAG_MANDATORY, (size_t) 1, cd_values);
    }else{
        printf("Filter %d not found. Not applied.\n", H5Z_FILTER_LZ4);
    }

    fspace_id = H5Screate_simple(rank, dims, NULL);

    dset_id = H5Dcreate(file_id, "LZ4 parallel", H5T_NATIVE_INT, fspace_id, H5P_DEFAULT, dcpl_id, H5P_DEFAULT);

    printf("Beginning write...\n");
    H5Dwrite_filter_parallel(dset_id, H5S_ALL, dset, 4);
    printf("Write completed.\n");

    free(dset);

    H5Dclose(dset_id);
    H5Sclose(fspace_id);
    H5Fclose(file_id);
    return 0;
}

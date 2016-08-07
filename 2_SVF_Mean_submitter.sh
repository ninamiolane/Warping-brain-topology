#!/bin/bash

ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l
N_mean=`ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l`

oarsub -S "/home/nmiolane/cluster/Scripts/OASIS_full/2_SVF_Mean_jobfile.pbs ${N_mean}"  -d /home/nmiolane/cluster/Scripts/OASIS_full/BCH_Atlas_t0_guimond



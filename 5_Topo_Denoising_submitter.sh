#!/bin/bash

ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l
N_mean=`ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l`


for f in /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_temp_p1.2_*nii.gz; do
echo "Gunzip atlas..."
gunzip "$f"
done

# Gunzip registered Subjects
for f in /home/nmiolane/cluster/Scripts/OASIS_full/BCH_Atlas_t0_guimond/Final_Image_*_p1.2.nii.gz; do
#echo "Gunzip subject image..."
gunzip "$f"
done

iter2=`expr ${N_mean} + 1`
oarsub -S "/home/nmiolane/cluster/Scripts/OASIS_full/5_Topo_Denoising_jobfile.pbs ${iter2}"  -d /home/nmiolane/cluster/Scripts/OASIS_full/BCH_Atlas_t0_guimond


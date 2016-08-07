#!/bin/bash

ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l
N_mean=`ls -l /home/nmiolane/cluster/Scripts/OASIS_full//MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l`

List_subjects='/home/nmiolane/cluster/Scripts/OASIS_full/List_All.txt'

for i in $(cat ${List_subjects});do
	oarsub -S "/home/nmiolane/cluster/Scripts/OASIS_full/3_Fast_Resampling_Registration_jobfile.pbs ${i} ${N_mean}" -d /home/nmiolane/cluster/Scripts/OASIS_full/BCH_Atlas_t0_guimond
done



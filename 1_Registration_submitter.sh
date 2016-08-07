#!/bin/bash

N_mean=8
atlasFolder="/home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases"
atlasName="Recon2D_p1.2_${N_mean}.nii"
atlasFile=${atlasFolder}/${atlasName}.gz
subjectsFolder="/home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Subjects2D"

# Gzip all images for registration
for f in ${subjectsFolder}/Real_*; do
gzip "$f"
done

# Gzip all images for registration
for f in ${subjectsFolder}/Bin_*; do
gzip "$f"
done

gzip ${atlasFolder}/${atlasName}

List_subjects='/home/nmiolane/cluster/Scripts/OASIS_full/List_All.txt' 	
	
ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l

N_mean=`ls -l /home/nmiolane/cluster/Scripts/OASIS_full/MorseSmale/Atlases/Recon2D_p1.2_*.nii.gz | wc -l`

for i in $(cat ${List_subjects});do
	oarsub -S "/home/nmiolane/cluster/Scripts/OASIS_full/1_Registration_jobfile.pbs ${i} ${N_mean}"  -d /home/nmiolane/cluster/Scripts/OASIS_full/BCH_Atlas_t0_guimond
done;

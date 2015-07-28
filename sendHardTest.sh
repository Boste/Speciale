#! /bin/bash
#
# Name of the job
#SBATCH -J test-2nodes -N 1 -n 1 --mem=25600 --gres=gpu:0 
#SBATCH -t 0:10:00 

#
# Use two nodes with 40 cores in total
#
# Memory: Use at most one of these two options
# MB memory/node. Use 256000 to get fat nodes

# MB memory/core. Use 12800 to get fat nodes, 20*12.8 ~= 256 GB
##SBATCH --mem-per-cpu=12800
#
# Request access to two GPU cards

#
# Max Walltime 1h

# Max Walltime 7days-0hours
##SBATCH -t 7-0
#
# Remove one # to run on the express node instead
# Note: the express node only has 1 GPU card
##SBATCH -p express
##SBATCH --gres=gpu:1
#
# Send email 
# Valid types are BEGIN, END, FAIL, REQUEUE, and ALL
#SBATCH --mail-type=ALL
# By default send email to the email address specified when the user was
# created this can be overrided here
#SBATCH --mail-user=boste09@student.sdu.dk 
#
# Write stdout/stderr output, %j is replaced with the job number
# use same path name to write everything to one file
#SBATCH --output slurm-%j.txt
#SBATCH --error  slurm-%j.txt

test -n "$SCRATCH" || export SCRATCH=/scratch

echo Running on $(hostname)
echo Available nodes: $SLURM_NODELIST
echo Slurm_submit_dir: $SLURM_SUBMIT_DIR
echo Start time: $(date)
# cd $SLURM_SUBMIT_DIR # not necessary, is done by default

# Load relevant modules
#module purge
#module add amber/14.2014.04
module load gcc/4.9.0




# Copy all input files to local scratch on all nodes
#for f in *.inp *.prmtop *.inpcrd ; do
#    sbcast $f $SCRATCH/$f
#done

export INSTANCE=$SLURM_SUBMIT_DIR/data/p6b.mps.gz
export EXECUTABLE=$SLURM_SUBMIT_DIR/src/xyz


for f in $INSTANCE $EXECUTABLE; do
    sbcast $f $SCRATCH/`basename $f`;
done


cd $SCRATCH

if [ "${CUDA_VISIBLE_DEVICES:-NoDevFiles}" != NoDevFiles ]; then
    echo "We have access to at least one GPU"
    cmd=pmemd.cuda.MPI
else
    echo "no GPUs available"
    cmd=pmemd.MPI
fi

export INPF=$SCRATCH/input
export OUPF=$SCRATCH/input
mpirun \
    ./xyz p6b.mps.gz -o $SLURM_SUBMIT_DIR/em.out 
echo Done.


ls 
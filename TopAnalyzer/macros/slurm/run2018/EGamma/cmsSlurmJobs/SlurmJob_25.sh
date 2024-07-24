#!/bin/sh
#SBATCH  -A cms
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=32000
#SBATCH --time=99:59:00
cd /depot/cms/top/chawla19/Topbjets/CMSSW_10_6_32/src/
source /cvmfs/oasis.opensciencegrid.org/osg-software/osg-wn-client/3.6/current/el7-x86_64/setup.sh
export X509_USER_PROXY=~/x509up_u`id -u`
source /cvmfs/cms.cern.ch/cmsset_default.sh
export BOOST_ROOT=/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/boost/1.63.0-gnimlf
export SCRAM_ARCH=slc7_amd64_gcc820
eval `scramv1 runtime -sh`
cd TopBJets/TopAnalyzer/macros/slurm/run2018/EGamma
echo 'cd outfiles1 && g++ `root-config --cflags --glibs` -lGenVector DNNInput1.C ConstrainedFit.cc -o fit1 && ./fit1'
cd outfiles1 && g++ `root-config --cflags --glibs` -lGenVector DNNInput1.C ConstrainedFit.cc -o fit1 && ./fit1
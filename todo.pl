#! /usr/bin/perl
use Cwd;
use POSIX;
use POSIX qw(strftime);

#############################################
$numArgs = $#ARGV +1;
$ARGV[$argnum];

$UserID= POSIX::cuserid();
$UserIDCern=$UserID;
$UserDir="";

if($UserID eq "vcherepa"){
    $UserIDCern="cherepan";
}


$PWD=getcwd;


if($ARGV[0] eq "--help" || $ARGV[0] eq ""){
    printf("\n\n\n ========================================================================================");
    printf("\nWelcome to tauola installer, please look at the instruction below.");
    printf("\nThis code requires one input option. The syntax is: ./todo.pl [OPTION]");
    printf("\n First of all setup the environment: \n\n");
    printf("\n./todo.pl --setup  <install>                              For example: ./todo.pl --setup  workdir\n");
    printf("\n\nAfter this step is completed prcoceed further and  ");
    printf("\nchoose from the following options:\n");
    printf("\n./todo.pl --help                                             Prints this message");
    printf("\n./todo.pl --install        <install>                              Install tauola and user codes; <install> must be the same as was set with setup option ");
    printf("\n  ========================================================================================\n");
    exit(0);  
}
my $dir = getcwd;
$time= strftime("%h_%d_%Y",localtime);



for($l=0;$l<$numArgs; $l++){
    
    if($ARGV[$l] eq "--setup"){
	$setdir=$ARGV[l+1];


	system(sprintf("rm install_env_*"));
	$SLDP='\$LD_LIBRARY_PATH';


	system(sprintf("rm install_env_*"));

	system(sprintf("echo \"export LD_LIBRARY_PATH=$SLDP:$PWD/$setdir/tauola++/1.1.5/TauSpiner/lib\">> install_env_$time"));
	system(sprintf("echo \"export LD_LIBRARY_PATH=$SLDP:$PWD/$setdir/tauola++/1.1.5/pythia8/176/lib/\">> install_env_$time"));
	system(sprintf("echo \"export LD_LIBRARY_PATH=$SLDP:$PWD/$setdir/tauola++/1.1.5/HepMC-2.06.05/workdir/lib \">> install_env_$time"));
	system(sprintf("echo \"export LD_LIBRARY_PATH=$SLDP:$PWD/$setdir/tauola++/1.1.5/lhapdf-5.9.1/workdir/lib\">> install_env_$time"));
	system(sprintf("echo \"export LD_LIBRARY_PATH=$SLDP:$PWD/$setdir/tauola++/1.1.5/examples/UserCodes\">> install_env_$time"));
	system(sprintf("echo \"cmsrel CMSSW_10_4_0; cd CMSSW_10_4_0/src/; cmsenv; cd ../../;\">> install_env_$time"));
	system(sprintf("echo \"export PYTHIA8='$PWD/$setdir/tauola++/1.1.5/pythia8/176/'\">> install_env_$time"));
	system(sprintf("echo \"export PYTHIA8DATA='$PWD/$setdir/tauola++/1.1.5/pythia8/176/xmldoc'\">> install_env_$time"));
#system(sprintf("echo \"cernlib-use --version 5.34.18 root \n\">> Install_TauolaEnvironment_$time"));
#	system(sprintf("echo \"source /cvmfs/cms.cern.ch/slc6_amd64_gcc700/lcg/root/6.12.07-ikaegh3/bin/thisroot.sh \n\">> install_env_$time"));


#	system(sprintf("cp Makefile.template Makefile; "));
#	system(sprintf("./subs '{DIR}'  $PWD/$setdir/  Makefile; "));

	printf("\n\nInstructions:");
	printf("\nTo complete this step do:  \n\n");
	printf("\n    1) source  install_env_$time \n");
	printf("\n    2) ./todo.pl --install $setdir \n\n");
    }


    if($ARGV[$l] eq "--install"){
	$tauoladir=$ARGV[l+1];
#$l++;
	$currentdir=getcwd;
	$s1_par='\$(pwd)';
	$SP='\$outfile';

#	system(sprintf("cernlib-use --version 5.34.18 root \n"));

	system(sprintf("rm -rf $tauoladir \n"));
	printf("\nInstalling sfotware  to  $tauoladir \n");
	system(sprintf("tar -xvf PythiaTauola.tar;"));
	system(sprintf("mkdir  $tauoladir \n"));
	system(sprintf("cd $tauoladir; cp ../PythiaTauolaSoft/tauola++-1.1.5-src.tgz .; tar -xzvf tauola++-1.1.5-src.tgz;"));
#	system(sprintf("cd $tauoladir; wget  http://tauolapp.web.cern.ch/tauolapp/resources/TAUOLA.1.1.5/TAUOLA.1.1.5-LHC.tar.gz; tar -xzvf TAUOLA.1.1.5-LHC.tar.gz;"));
	system(sprintf("cd $tauoladir/tauola++/1.1.5/; cp ../../../PythiaTauolaSoft/pythia8-176-src.tgz .; tar xvfz pythia8-176-src.tgz;"));
	system(sprintf("cd $tauoladir/tauola++/1.1.5/; cp ../../../PythiaTauolaSoft/lhapdf-5.9.1.tar.gz .; tar -xzvf lhapdf-5.9.1.tar.gz;"));
	system(sprintf("cd $tauoladir/tauola++/1.1.5/; cp ../../../PythiaTauolaSoft/MC-TESTER-1.25.0.tar.gz .; tar -xzvf MC-TESTER-1.25.0.tar.gz;"));
	system(sprintf("cd $tauoladir/tauola++/1.1.5/; cp ../../../PythiaTauolaSoft/HepMC-2.06.05.tar.gz .;  tar -xzvf  HepMC-2.06.05.tar.gz;"));

	printf("\n Downloading Complete ... \n");
	printf("\n Start Installation ... \n\n\n");
	printf("\n ___________________Installing HEPMC ... _____________________\n\n\n");
	system(sprintf("mkdir $PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir; .././configure -prefix=$PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir  -with-momentum=GEV -with-length=CM; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir; make; make check; make install;"));
	printf("\n___________________Installing  LHAPDF ... _____________________\n\n\n");
	system(sprintf("mkdir $PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/; ./configure -prefix=$PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir  --libdir=$PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir/lib "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/; make;  make install;"));
	system(sprintf("mkdir  $PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir/share/lhapdf/PDFsets;"));
	system(sprintf("cd  $PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir/share/lhapdf/PDFsets; ../../../bin/lhapdf-getdata --repo=http://www.hepforge.org/archive/lhapdf/pdfsets/5.9.1 MSTW2008nnlo90cl.LHgrid; "));
	printf("\n ___________________Installing pythia8 ... _____________________ \n\n\n");

	system(sprintf("mkdir $PWD/$tauoladir/tauola++/1.1.5/pythia8/176/workdir; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/pythia8/176/;  ./configure --enable-shared --lcgplatform=slc6_amd64_gcc530-opt --with-hepmc=$PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir --with-hepmcversion=2.06.05;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/pythia8/176/;  make; "));
	system(sprintf("cp -r UserArea/ $PWD/$tauoladir/tauola++/1.1.5/pythia8/176/; "));

	printf("\n ___________________Compiling MC-TESTER ... _____________________\n\n\n");
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/MC-TESTER/;  ./configure --with-HepMC=$PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/MC-TESTER/;  make; "));
	printf("\n ___________________Compiling tauola ... _____________________\n\n\n");

	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; export PYTHIA8DATA=$PWD/$tauoladir/tauola++/1.1.5/pythia8/176/share/Pythia8/xmldoc;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/TauSpiner/tauola++/1.1.5/lib;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/$tauoladir/tauola++/1.1.5/pythia8/176/lib/;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/;export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir/lib; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir/lib"));
	system(sprintf("mkdir $PWD/$tauoladir/tauola++/1.1.5/workdir; "));
#	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/;   ./configure --prefix=$PWD/$tauoladir/tauola++/1.1.5/workdir  --with-hepmc=$PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir  --with-pythia8=$PWD/$tauoladir/tauola++/1.1.5/pythia8/176/  --with-lhapdf=$PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir/ --with-mc-tester=$PWD/$tauoladir/tauola++/1.1.5/MC-TESTER/; "));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/;   ./configure --prefix=$PWD/$tauoladir/tauola++/1.1.5/workdir  --with-hepmc=$PWD/$tauoladir/tauola++/1.1.5/HepMC-2.06.05/workdir  --with-pythia8=$PWD/$tauoladir/tauola++/1.1.5/pythia8/176/  --with-lhapdf=$PWD/$tauoladir/tauola++/1.1.5/lhapdf-5.9.1/workdir/; "));

	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; make;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; make all;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/; make install;"));
	system(sprintf("cd $PWD/$tauoladir/tauola++/1.1.5/examples; make"));
    }
}

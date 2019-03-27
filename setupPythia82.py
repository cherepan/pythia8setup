#!/usr/bin/env python

import os
import sys
import argparse



if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-w", "--work-dir",help="working directory ", type = str, action="store", default = 'workdir')
    args = parser.parse_args()
    cwd = os.getcwd()


    if os.path.isdir(args.work_dir):
        print "Folder %s already exists, please change tag name or delete it" % args.work_dir
        sys.exit()
    os.system('mkdir '+ args.work_dir)
    getpythia= 'cd '+ args.work_dir+ ' ;wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8240.tgz;'
    comp = 'cd '+ args.work_dir+ ' ;tar xvfz pythia8240.tgz;'  + 'tar xvfz pythia8240.tgz;' + 'cd pythia8240;' + 'make'
    os.system(getpythia)
    os.system(comp)


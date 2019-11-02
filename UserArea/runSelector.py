#!/usr/bin/env python


import ROOT
import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-t', dest='tag', type=str, help='for each plot')
    parser.add_argument('--doME11', dest='doME11', action='store_true', default=False)
    parser.add_argument('--inputfile', dest='inputfile', type=str, help='inputfile')

    args = parser.parse_args()
    return args

args=ParseOption()

filename = "outputPythiaTreeTest.root"
#filename = "DUMMYFILENAME_RU.root"
tag = args.tag
doME11 = args.doME11 
#tag = "RU"

#filename = "DUMMYFILENAME_"+tag+".root"
filename = args.inputfile



#treename = "cscRootMaker/Events"
treename = "T"
selectorname = "PythiaSelector.C"

savedir = "DUMMY" #"/home/mhl/public_html/2017/20171017_cscSeg/"

f_ = ROOT.TFile.Open(filename)
t_ = f_.Get(treename)
s_ = ROOT.TSelector.GetSelector(selectorname)
#s_.SetInputs(tag,savedir,doME11)
#s_.SetInputs(tag,doME11)
t_.Process(s_)

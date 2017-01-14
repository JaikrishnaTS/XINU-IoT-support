#!/usr/bin/env python

import xml.etree.ElementTree
from sys import argv, exit
import os, shutil
import fileinput

if len(argv) < 3:
    print "Usage: Use the Xinu source code along with any device driver code to configure the OS with specific device"
    print "Format:", argv[0], "<device folder> <Xinu codebase>"
    exit(-1)

devdir = argv[1]
xinudir = argv[2]
devfiles = os.listdir(devdir)

dcfile = os.path.join(devdir, "devconf.xml")
if not os.path.isfile(dcfile):
    print "Device configuration - devconf.xml not found"
    exit(-1)

e = xml.etree.ElementTree.parse(dcfile).getroot()

devconf = {}
for conf in e:
    devconf[conf.tag] = conf.text

xinudevdir = (os.path.join(xinudir, "device", devconf['name']))
if not os.path.exists(xinudevdir):
    os.mkdir(xinudevdir)

for devfile in devfiles:
    if devfile.endswith(".c"):
        shutil.copy(os.path.join(devdir, devfile), xinudevdir)
    if devfile.endswith(".h"):
        shutil.copy(os.path.join(devdir, devfile), os.path.join(xinudir, "include"))

with open(os.path.join(xinudir, "include", "xinu.h"), 'a') as includefile:
    includefile.write("\n#include <" + devconf['name'] + ".h>")

ln = 0
for line in fileinput.input(os.path.join(xinudir, "config", "Configuration"), inplace=True):
    if line.startswith("%%"):
        if ln == 0:
            line = devconf['name'] + ": on " + devconf['depends'] + " -i " + devconf['init'] + " -o " + devconf['open'] + " -c " + devconf['close'] + " -r " + \
                devconf['read'] + " -w " + devconf['write'] + " -s " + devconf['seek'] + " -g " + devconf["getc"] + " -p " + devconf['putc'] + " -n " + devconf['control'] + " -intr " + devconf['intr'] +"\n%%\n"
        elif ln == 1:
            line = devconf['devname'] + " is " + devconf['name'] + " on " + devconf['depends'] 
            if (devconf['csr']):
                line += " csr " + devconf['csr']
            if (devconf['irq']):
                line += " -irq " + devconf['irq']
            line += "\n%%\n"
        else: 
            print "some error in config"
            exit(-1)
        ln += 1
    print line,

for line in fileinput.input(os.path.join(xinudir, "compile", "Makefile"), inplace=True):
    if line.startswith("REBUILDFLAGS"):
        line += " -s $(TOPDIR)/device/" + devconf['name'] + '\\\n'
    print line,


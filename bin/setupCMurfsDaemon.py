#!/usr/bin/python

import sys
import os
import subprocess


rootDir = os.getenv("CMURFS_DIR")
robotDir = '/home/nao'

def copyFiles(robot):	        
	if os.system('scp -q ' + rootDir + '/bin/cmurfsd nao@' + robot + ':' + robotDir + '/cmurfsd') != 0:
		print 'Failed to copy cmurfsd to robot'
		exit(-1)
	else:
		print 'Copied cmurfsd to robot'
	if os.system('scp -q ' + rootDir + '/bin/installCMurfsd.sh nao@' + robot + ':' + robotDir + '/installCMurfsd.sh') != 0:
		print 'Failed to copy installCMurfsd.sh to robot'
		exit(-2)
	else:
		print 'Copied installCMurfsd.sh to robot'	

if len(sys.argv) < 2:
	print 'USAGE: setupUSB.py robotaddress'
	exit(-4)

robotIP = sys.argv[1]

copyFiles(robotIP)


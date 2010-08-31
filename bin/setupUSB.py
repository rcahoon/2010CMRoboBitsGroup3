#!/usr/bin/python

import sys
import os
import subprocess


def setupDir(robot):	        
	ret = os.system("ssh nao@" + robot + " mkdir /home/nao/naoqi/lib")
	if ret !=0:
		print 'Failed to create directory for /home/nao/naoqi/lib'
		exit(-1)
	else:
		print 'Created directory /home/nao/naoqi/lib'	

	ret = os.system("ssh nao@" + robot + " mkdir /home/nao/naoqi/lib/naoqi")
	if ret !=0:
		print 'Failed to create directory for /home/nao/naoqi/lib/naoqi'
		exit(-2)
	else:
		print 'Created directory /home/nao/naoqi/lib/naoqi'	
		
if len(sys.argv) < 2:
	print 'USAGE: setupUSB.py robotaddress'
	exit(-3)

setupDir(sys.argv[1])



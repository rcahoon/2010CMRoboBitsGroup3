#!/usr/bin/python

import sys
import os
import subprocess

def fileExists(f):
	try:
		file = open(f)
	except IOError:
		return 0
	else:
		return 1

home = os.path.expanduser('~')
sshdir = home + '/.ssh'
pubkeyfile = sshdir + '/id_rsa.pub'
privatekeyfile = sshdir + '/id_rsa'

name = os.environ['USERNAME']

def createKeys():
	if not fileExists(pubkeyfile) or not fileExists(privatekeyfile):
		print 'Generating ssh keys.'
		sshproc = subprocess.Popen(['ssh-keygen', '-t', 'rsa',
				'-f', privatekeyfile, '-N', '', '-q',
				'-C', name], 0, None)
		ret = sshproc.wait()
		if ret != 0:
			print 'Failed to generate keys, aborting.'
			exit(-1)

def sendKeys(robot):	        
	print 'Please enter the password when prompted.'
	os.system("ssh nao@" + robot + " mkdir /home/nao/.ssh 2>/dev/null")        
	print 'Getting key file.'
	ret = os.system('scp -q nao@' + robot + ':~/.ssh/authorized_keys2 ' + 'temp.txt')
	if ret != 0:
		print 'Could not download keyfile. Creating new file.'
		try:
			keys = open('temp.txt', 'w')
			keys.close()
		except IOError:
			print 'Could not open temp.txt'
			exit(-1)
	try:
		# check if our key is already there
		keys = open('temp.txt', 'r')
		newkeys = open('temp2.txt', 'w')
		mykeyfile = open(pubkeyfile, 'r')
		mykey = mykeyfile.readline()
		for line in keys:
			# if our key is already there, don't write it twice
			# we will overwrite it later
			if not line.endswith(' ' + name):
				newkeys.write(line)

		newkeys.write(mykey)
		
		keys.close()
		newkeys.close()
		mykeyfile.close()

		print 'Sending new key file.'
		ret = os.system('scp -q temp2.txt nao@' + robot + ':~/.ssh/authorized_keys2')
		if ret != 0:
			print 'Could not send new key file to ' + robot + '.'
			exit(-1)

		os.remove('temp.txt')
		os.remove('temp2.txt')

	except IOError:
		print 'Could not open temp.txt'
		exit(-1)

if len(sys.argv) < 2:
	print 'USAGE: sshsetup.py robotaddress'
	exit(-2)

createKeys()
sendKeys(sys.argv[1])



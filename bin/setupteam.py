#!/usr/bin/python

# Pass the names of the robots to setup to this script.
#
# This script updates the config direction, motion module,
# main module and autoload.ini of the specified robots. It
# also recompiles the main module and motion module.
# Use --nomotion to not update the motion module, and
# --nomain to not update the main module.
# Use --offboard or -o to prepare for offboard main module execution.

# To generate keys:
#  ssh-keygen -t rsa
#  append contents of id_rsa.pub to robot's .ssh/authorized_keys2
#
# or use sshsetup.py
#

import os, sys
from optparse import OptionParser


# tuple of hostname, player number, team number, config file, robot name
#robots = [('nao-athena.wv.cc.cmu.edu',     1, 10, 'athena_config.txt', 'athena'),
#          ('nao-ares.wv.cc.cmu.edu',       2, 10, 'ares_config.txt', 'ares'),
#          ('nao-artemis.wv.cc.cmu.edu',    3, 10, 'artemis_config.txt', 'artemis'),
#          ('nao-apollo.wv.cc.cmu.edu',     4, 10, 'apollo_config.txt', 'apollo'),
#          ('nao-aphrodite.wv.cc.cmu.edu',  5, 10, 'aphrodite_config.txt', 'aphrodite'),
#          ('nao-anubis.wv.cc.cmu.edu',     6, 10, 'anubis_config.txt', 'anubis'),
#         ('nao-apophis.wv.cc.cmu.edu',    7, 10, 'apophis_config.txt', 'apophis'),
#         ]

rootDir = os.getenv("CMURFS_DIR")
configDir = rootDir + '/robot/config/'
robotDir = '/home/nao'
robotConfigDir = robotDir + '/config'
robotNaoqiDir = robotDir + '/naoqi/lib/naoqi'
robotAutoloadDir = robotDir + '/naoqi/preferences'
mainDir = rootDir + '/robot/Main'
mainBin = mainDir + '/bin/cmurfs'
naoqiModuleDir = rootDir + '/robot/NaoQiModule'
naoqiModuleBin = naoqiModuleDir + '/bin/libcmurfs.so'

def build_Main():
	print 'Building Main Module!\n'
	
	if os.system('make -C ' + mainDir) != 0:
		print 'Failed to build MainModule. Aborting.'
		exit(-1)
	
	print 'Main Module built successfully!\n'

def build_Naoqi():
	print 'Building Naoqi Module!\n'
	
	if os.system('make -C ' + naoqiModuleDir) != 0:
		print 'Failed to build Naoqi Module. Aborting.'
		exit(-1)
	
	print 'Naoqi Module built successfully!\n'


def stop_Naoqi(address):
    print 'Disabling naoqi.'
    if os.system('ssh -q nao@' + address + ' \"/etc/init.d/naoqi stop < /dev/null > /dev/null 2>&1 &\"') != 0:
        print 'Failed to disable naoqi.'
        exit(-1)
  
def start_CMurfs(address):
    print 'Stopping CMurfs.'
    if os.system('ssh -q nao@' + address + ' \"/etc/init.d/cmurfsd start < /dev/null > /dev/null 2>&1 &\"') != 0:
        print 'Failed to stop CMurfs.'
        exit(-1)
                
def stop_CMurfs(address):
    print 'Stopping CMurfs.'
    if os.system('ssh -q nao@' + address + ' \"/etc/init.d/cmurfsd stop < /dev/null > /dev/null 2>&1 &\"') != 0:
        print 'Failed to stop CMurfs.'
        exit(-1)
                
def copy_Config(address):
    print 'Copying config directory'
    if os.system('scp -q -r ' + configDir + ' nao@' + address + ':' + robotDir) != 0:
        print 'Failed to copy config directory to robot.'
        exit(-1)
    
def copy_Main(address):
    print 'Copying main module.'
    if os.system('scp -q ' + mainBin + ' nao@' + address + ':' + robotDir) != 0:
        print 'Failed to copy Main Module to robot.'
        exit(-1)

def copy_Naoqi(address):
    print 'Copying Naoqi module.'
    if os.system('scp -q ' + naoqiModuleBin + ' nao@' + address + ':' + robotNaoqiDir) != 0:
        print 'Failed to copy Naoqi Module to robot.'
        exit(-1)
            
def copy_Autoload(address):
    print 'Copying autoload.ini.'
    if os.system('scp -q ' + rootDir + '/bin/autoload.ini nao@' + address + ':' + robotAutoloadDir + '/autoload.ini') != 0:
        print 'Failed to copy autoload.ini to robot.'
        exit(-1)
    print 'Copying autoload.sh.'
    if os.system('scp -q ' + rootDir + '/bin/autoload.sh nao@' + address + ':' + robotDir + '/autoload.sh') != 0:
        print 'Failed to copy autoload.ini to robot.'
        exit(-1)
                
def restart_Naoqi(address):
    print 'Restarting naoqi.'
    # don't ask me why, but this makes ssh not wait for it to finish.
    if os.system('ssh -q nao@' + address + ' \"/etc/init.d/naoqi restart < /dev/null > /dev/null 2>&1 &\"') != 0:
        print 'Failed to start naoqi.'
        exit(-1)

def copy_Identity_Text(address, teamNumber, playerNumber, robotIdentityText):
	identityFileName = robotIdentityText
	f = open(identityFileName, 'w')
	f.write('# Identity settings\n')
	f.write('# -----------------\n')
	f.write('team/teamNumber = ' + str(teamNumber) + '\n')
	f.write('team/playerNumber = ' + str(playerNumber) + '\n')
	f.write('# Override on the robot as necessary\n')
	f.write('~INCLUDE ' + str(robotIdentityText) + '\n')
	f.close()

	print 'Copying identity file.'
	if os.system('scp -q ' + identityFileName + ' nao@' + address + ':' + robotConfigDir + '/identity.txt') != 0:
		print 'Failed to copy identity.txt to robot.'
		exit(-1)
    
	os.remove(identityFileName)
     
parser = OptionParser(usage="%prog [--naoqi] [--main] [--copyConfig] [--copyNaoqi] [--copyMain] [--stopNaoqi] [--restartNaoqi] [--startCMurfs] [--stopCMurfs] [--copyAutoload] [--copyIdentity] [--ip robotIP] [--teamNum teamNumber] [--playerNum playerNumber] [--robotIdentityFile robotIdentityFile]", version="%prog 2.0")
parser.set_defaults(naoqi=False, main=False, copyConfig=False, copyNaoqi=False, copyMain=False, stopNaoqi=False, restartNaoqi=False, startCMurfs=False, stopCMurfs=False, copyAutoload=False, copyIdentity=False)
parser.add_option("--naoqi", action="store_true",
		dest="naoqi", help="Build the naoqi module.")
parser.add_option("--main", action="store_true",
		dest="main", help="Build the main module.")
parser.add_option("--copyConfig", action="store_true",
        dest="copyConfig", help="Copying all config")
parser.add_option("--copyNaoqi", action="store_true",
        dest="copyNaoqi", help="Copy Naoqi.")
parser.add_option("--copyMain", action="store_true",
        dest="copyMain", help="Copy Main.")
parser.add_option("--stopNaoqi", action="store_true",
        dest="stopNaoqi", help="Stop Naoqi")
parser.add_option("--restartNaoqi", action="store_true",
        dest="restartNaoqi", help="Restart Naoqi.")
parser.add_option("--startCMurfs", action="store_true",
        dest="startCMurfs", help="Start CMurfs.")
parser.add_option("--stopCMurfs", action="store_true",
        dest="stopCMurfs", help="Stop CMurfs.")
parser.add_option("--copyAutoload", action="store_true",
        dest="copyAutoload", help="Copy Autoload.ini.")
parser.add_option("--copyIdentity", action="store_true",
        dest="copyIdentity", help="Copy Identity.txt.")
parser.add_option("--ip", action="store",
        dest="ip", help="Set the robot's ID.")
parser.add_option("--teamNum", action="store",
        dest="teamNum", help="Set the robot's team number.")
parser.add_option("--playerNum", action="store",
        dest="playerNum", help="Set the robot's player number.")
parser.add_option("--robotIdentityFile", action="store",
        dest="robotIdentityFile", help="Set the robot's identity text file.")

(opts, args) = parser.parse_args()

naoqi = opts.naoqi
main = opts.main
copyConfig = opts.copyConfig
copyNaoqi = opts.copyNaoqi
copyMain = opts.copyMain
stopNaoqi = opts.stopNaoqi
restartNaoqi = opts.restartNaoqi
startCMurfs = opts.startCMurfs
stopCMurfs = opts.stopCMurfs
copyAutoload = opts.copyAutoload
copyIdentity = opts.copyIdentity
ip = opts.ip
teamNum = opts.teamNum
playerNum = opts.playerNum
robotIdentityFile = opts.robotIdentityFile

if naoqi:
	build_Naoqi()

if main:
	build_Main()

if copyAutoload:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        copy_Autoload(ip)    

if copyIdentity and not copyConfig:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    elif teamNum == None or teamNum == "":
        print 'No team number specified'
        exit(-1)
    elif playerNum == None or playerNum == "":    
        print 'No player number specified'
        exit(-1)
    elif robotIdentityFile == None or robotIdentityFile == "":
    	print 'No robot identity file specified'
    	exit(-1)
    else:
        copy_Identity_Text(ip, teamNum, playerNum, robotIdentityFile)

if copyConfig and not copyIdentity:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        copy_Config(ip)

if copyIdentity and copyConfig:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    elif teamNum == None or teamNum == "":
        print 'No team number specified'
        exit(-1)
    elif playerNum == None or playerNum == "":    
        print 'No player number specified'
        exit(-1)
    elif robotIdentityFile == None or robotIdentityFile == "":
    	print 'No robot identity file specified'
    	exit(-1)
    else:
    	copy_Config(ip)
        copy_Identity_Text(ip, teamNum, playerNum, robotIdentityFile)

# Make sure that the order of the functions are in this order else Deployer will mess up
if stopCMurfs:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        stop_CMurfs(ip)

if stopNaoqi:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        stop_Naoqi(ip)

if copyMain:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        copy_Main(ip)

if copyNaoqi:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        copy_Naoqi(ip) 

if restartNaoqi:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        restart_Naoqi(ip)

if startCMurfs:
    if ip == None or ip == "":
        print 'No IP address specified'
        exit(-1)
    else:
        start_CMurfs(ip)
        
#print sys.argv[1]

#for name in args:
#	for r in robots:
#		if name == r[4]:
#			setupRobot(r[1], r[2], r[3], r[0], main, motion, offboard, sentinel)
#			break


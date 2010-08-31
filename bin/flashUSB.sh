if [ $# == 0 ] 
then
echo "Usage: flashUSB.sh {path_to_ext3_file} {sudo_password}"
exit 1;
fi
sudo -S $NAOQI_DIR/bin/flash-usbstick $1 
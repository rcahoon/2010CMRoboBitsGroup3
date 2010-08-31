cd $CMURFS_DIR/robot
if [ $# -eq 0 ]
then
  scp -r config/ nao@$NAO_IP:/home/nao/
else
  scp -r config/ nao@$1:/home/nao/
fi

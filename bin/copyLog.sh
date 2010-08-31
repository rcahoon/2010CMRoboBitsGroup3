if [ $# -lt 2 ]; then 
  echo "Usage: copyLog.sh {nao_ip} {path_to_copy_log_file_to}"
  exit 1;
else
  scp nao@$1:/home/nao/logToFile.log $2
fi


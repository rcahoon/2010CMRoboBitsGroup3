#!/bin/sh

case "$1" in
    install)
        cp /home/nao/cmurfsd /etc/init.d/
        echo "alias cmurfs='/etc/init.d/cmurfsd'" >> /etc/profile.d/nao-profile.sh
    ;;
    
    uninstall)
        rm -f /etc/init.d/cmurfsd
        sed '/cmurfsd/d' /etc/profile.d/nao-profile.sh > /etc/profile.d/temp.txt
        mv /etc/profile.d/temp.txt /etc/profile.d/nao-profile.sh
    ;;

    autostart-on)
        ln -s /etc/init.d/cmurfsd /etc/rc0.d/K03cmurfsd
        ln -s /etc/init.d/cmurfsd /etc/rc1.d/K03cmurfsd
        ln -s /etc/init.d/cmurfsd /etc/rc2.d/S59cmurfsd
        ln -s /etc/init.d/cmurfsd /etc/rc3.d/S59cmurfsd
        ln -s /etc/init.d/cmurfsd /etc/rc4.d/S59cmurfsd
        ln -s /etc/init.d/cmurfsd /etc/rc5.d/S59cmurfsd
        ln -s /etc/init.d/cmurfsd /etc/rc6.d/K03cmurfsd
    ;;

    autostart-off)
        rm -f /etc/rc0.d/K03cmurfsd
        rm -f /etc/rc1.d/K03cmurfsd
        rm -f /etc/rc2.d/S59cmurfsd
        rm -f /etc/rc3.d/S59cmurfsd
        rm -f /etc/rc4.d/S59cmurfsd
        rm -f /etc/rc5.d/S59cmurfsd
        rm -f /etc/rc6.d/K03cmurfsd
    ;;

    *)
        echo "Usage cmurfsSetup.sh {install|uninstall|autostart-on|autostart-off}"
        echo "You have to run this script as root"
        exit 1;
    ;;
esac

exit 0

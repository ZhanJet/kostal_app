#!/bin/sh
echo "Startup Kostal Client."
echo Kostal8888 | sudo -S su > /dev/null 2>&1
sudo -E /home/kostal/Programs/Kostal/flexiv_app_guis/build/app/kostal_rdk_client/kostal_rdk_client

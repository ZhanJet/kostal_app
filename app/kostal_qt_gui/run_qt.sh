#!/bin/sh
echo "Startup Kostal Client."
echo rizon.2019 | sudo -S su > /dev/null 2>&1
sudo -E /home/kostal/Programs/Kostal/flexiv_app_guis/build/app/kostal_qt_gui/kostal_qt_gui

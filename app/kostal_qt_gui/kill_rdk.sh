#!/bin/sh
echo rizon.2019 | sudo -S su > /dev/null 2>&1
sudo -E kill -9 $(pgrep -f rdk)

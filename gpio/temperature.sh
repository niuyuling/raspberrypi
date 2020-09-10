#!/bin/bash
DATE=$(date "+%y年%m月%d日%H时%M分%S秒");
T="$(/root/gpio/wind -d 28-031682c7baff)"
echo "${DATE} ${T}" >> /root/temperature.data

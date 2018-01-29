#!/bin/bash
#
# reapberry zero w
# GPIO module
# Date: 20180118
# Time: 17:17
#
# AUthor: aixiao@aixiao.me.
#

function DATE()
{
    y=$(date "+%y");
    m=$(date "+%m");
    d=$(date "+%d");
    H=$(date "+%H");
    M=$(date "+%M");
    S=$(date "+%S");
}

function SLEEP()
{
    sleep $@
}

function get18b20data()
{
    MODEL="28-04174019f0ff";
    if test -d "/sys/devices/w1_bus_master1/${MODEL}"; then
        temperature=$(cat /sys/devices/w1_bus_master1/28-04174019f0ff/w1_slave | grep 't=' | sed 's/^.*t=//g' | awk '{print $1/1000}');
        #echo $y年$m月$d日$H时$M分$S秒
        echo -ne "当前室内温度: ";
        echo -e "\e[1;31m${temperature}℃ \e[0m";
    else
        return 1;
    fi
}

function getcpudata()
{
    MHZ=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq | awk '{print $1/1000}');
    temperature=$(cat /sys/class/thermal/thermal_zone0/temp | awk '{print $1/1000}');
    percentage=$(top -b -n 2 | grep ^%Cpu | sed -n "2p" | sed -e 's/^.*://g;s/..,//g;s/st//g' | awk '{print "scale=1; 100-" $4 }' | bc);
    echo "Cpu Frequencyi:  ${MHZ}MHZ";
    echo -e "\e[1;31mCpu temperature: ${temperature}℃ \e[0m";
    echo "Cpu Percentage:  ${percentage}%";
}

function lightinfo()
{
    bindir="/root/gpio"
    ! test -d ${bindir} && exit 1;
    #时间范围
    high="1500";
    low="2400";

    #当前时间
    now=$(date +%H%M);
    bcm_lightpin="20";
    wiringpi_lightpin="28";

    wiringpi_infrared="6";
    
    #判断手机是否在线
    #phoneip="192.168.137.27";
    #phonelogic="$(ping ${phoneip} -c 1 -S 1 &> /dev/null; echo $?)";

    #判断人体红外线传感器
    #infrared=$(${bindir}/info_infrared ${wiringpi_infrared});
    infrared=$(cat ${bindir}/infrared.log 2> /dev/null);

    #距离传感器
    l="60";
    s="150";
    wiringpi_tring="8";
    wiringpi_echo="9";
    #distance="$(${bindir}/info_ultrasound ${wiringpi_tring} ${wiringpi_echo})";

    #读取灯pin值
    lightpinvalue="$(${bindir}/info_pin ${wiringpi_lightpin})";
    #lightpinvalue="$(gpio -g read ${bcm_lightpin})";

    #开灯&关灯
    start_light="${bindir}/info_light ${wiringpi_lightpin} 1";
    stop_light="${bindir}/info_light ${wiringpi_lightpin} 0";

}

#DATE;
#getcpudata;
#get18b20data >> ~/temperature.txt;
lightinfo;


function WHILE()
{
    while true; do
        ${bindir}/info_infrared ${wiringpi_infrared} > infrared.log
        SLEEP 1
    done
}

function main_()
{
while true; do
    if [ "${now}" -ge "${high}" -a "${now}" -le "${low}" ]; then             #检测时间段
        #if [ "${phonelogic}" == "0" ]; then                                 #检测手机IP
        #echo ${infrared}
        if [ "${infrared}" = "1" ]; then                                     #检测人体红外线
        #if [ "${distance}" -ge ${l} -a "${distance}" -le ${s} ]; then       #检测距离范围
            if test "${lightpinvalue}" = "0"; then                           #检测灯pin值
                ${start_light};
                SLEEP 9;
            fi
        else
            if test "${lightpinvalue}" = "1"; then                  #如果开着就关闭
                ${stop_light};
            fi
        fi
    else
        if test "${lightpinvalue}" = "1"; then                      #如果开着就关闭
            ${stop_light};
        fi
    fi
    lightinfo;                                                      #再次获取信息
    SLEEP 1;                                                        #睡眠
done
}

while getopts :ds: pi
do
case ${pi} in
    d)
        daemon='&';
        ;;
    s)
        case $OPTARG in
            "stop")
                array=$(ls ${bindir}/log/)
                for arr in ${array[@]}; do
                    kill $(cat ${bindir}/log/${arr})
                done
                exit 0
                ;;
            *)
                :
                ;;
        esac
        exit 0
        ;;
    h|?)
        echo "
raspberrypi zero w 智能灯控
$0 Usage: $0 [-?|h] [-d] [-s]
    -d Daemon.
    -s kill Daemon, signal: stop.

by aixiao.
"
exit 0
        ;;
esac
done

WHILE &
WHILEID="$(echo $!)"
echo ${WHILEID} > log/infrared.pid 2> /dev/null
eval main_ ${daemon}
echo $! > log/main.pid 2> /dev/null

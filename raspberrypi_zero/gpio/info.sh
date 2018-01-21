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

function get18b20data()
{
    MODEL="28-04174019f0ff";
    if test -d "/sys/devices/w1_bus_master1/${MODEL}"; then
        temperature=$(cat /sys/devices/w1_bus_master1/28-04174019f0ff/w1_slave | grep 't=' | sed 's/^.*t=//g' | awk '{print $1/1000}');
        echo $y年$m月$d日$H时$M分$S秒
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
    high="1400";
    low="1430";
    #当前时间
    now=$(date +%H%M);
    bcm_lightpin="20";
    wiringpi_lightpin="28";
    
    #判断手机是否在线
    phoneip="192.168.137.175";
    phonelogic="$(ping ${phoneip} -c 1 -S 1 &> /dev/null; echo $?)";

    #读取灯pin值
    lightpinvalue="$(${bindir}/info_pin ${wiringpi_lightpin})";
    lightpinvalue="$(gpio -g read ${bcm_lightpin})";

    #开灯&关灯
    start_light="${bindir}/info_light ${wiringpi_lightpin} 1";
    stop_light="${bindir}/info_light ${wiringpi_lightpin} 0";

}

DATE;
#getcpudata;
#get18b20data >> ~/temperature.txt;
lightinfo;

while true; do
    if [ "${now}" -ge "${high}" -a "${now}" -le "${low}" ]; then    #检测时间段
        if [ "${phonelogic}" == "0" ]; then                         #检测手机IP
            if test "${lightpinvalue}" = "0"; then                  #检测灯pin值
                ${start_light};
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
    sleep 9;                                                        #睡眠
done


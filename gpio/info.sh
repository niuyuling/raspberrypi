#!/bin/bash
#
# reapberry zero w & pi 3.
# GPIO module
# Date: 20180118
# Time: 17:17
#  
#
# AUthor: aixiao@aixiao.me.
#

: 等待.
function SLEEP()
{
    sleep $@
}

: 18b20温度传感器.
function get18b20data()
{
    MODEL="28-04174019f0ff";
    while true; do
        if test -d "/sys/devices/w1_bus_master1/${MODEL}"; then
            temperature=$(cat /sys/devices/w1_bus_master1/28-031682c7baff/ | grep 't=' | sed 's/^.*t=//g' | awk '{print $1/1000}');
            #echo $y年$m月$d日$H时$M分$S秒
            #echo -ne "当前室内温度: ";
            #echo -e "\e[1;31m${temperature}℃ \e[0m";
            echo "${temperature}" >> ${bindir}/data/temperature.data;
        fi
        SLEEP 60;
    done
}

: CPU温度.
function getcpudata()
{
    MHZ=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq | awk '{print $1/1000}');
    temperature=$(cat /sys/class/thermal/thermal_zone0/temp | awk '{print $1/1000}');
    percentage=$(top -b -n 2 | grep ^%Cpu | sed -n "2p" | sed -e 's/^.*://g;s/..,//g;s/st//g' | awk '{print "scale=1; 100-" $4 }' | bc);
    echo "Cpu Frequencyi:  ${MHZ}MHZ";
    echo -e "\e[1;31mCpu temperature: ${temperature}℃ \e[0m";
    echo "Cpu Percentage:  ${percentage}%";
}

function INIT()
{
    null="/dev/null";
    bindir="/root/gpio";
    ! test -d ${bindir} && exit 1;
	
	#时间
	y=$(date "+%y");
    m=$(date "+%m");
    d=$(date "+%d");
    H=$(date "+%H");
    M=$(date "+%M");
    S=$(date "+%S");
	W=$(date "+%w");	: 周几?
	
	start=1;
	stop=0;
	
    #时间范围
    high="1700";
    low="2300";

    #当前时间
    now=$(date +%H%M);
    
    #判断手机是否在线
    phoneip="192.168.99.60";
    phonelogic="$(ping ${phoneip} -c 1 -S 1 &> /dev/null; echo $?)";

    #判断人体红外线传感器
	wiringpi_infrared="6";
    #infrared=$(${bindir}/info_infrared ${wiringpi_infrared});
    infrared=$(cat ${bindir}/data/infrared.data 2> /dev/null);

    #声音传感器
    wiringpi_sound=25;
    sound=$(cat ${bindir}/data/sound.data 2> /dev/null);

    #距离传感器
    l="60";
    s="150";
    wiringpi_tring="8";
    wiringpi_echo="9";
    #distance="$(${bindir}/info_ultrasound ${wiringpi_tring} ${wiringpi_echo})";
{
    #读取灯pin值
    lightpinvalue="$(${bindir}/info_pin ${wiringpi_lightpin})";
    #lightpinvalue="$(gpio -g read ${bcm_lightpin})";

    #开灯&关灯
	wiringpi_lightpin="4";
    start_light="${bindir}/info_light ${wiringpi_lightpin} 1";
    stop_light="${bindir}/info_light ${wiringpi_lightpin} 0";
}
	#风扇
	wiringpi_wind="1";			#脚位
	l_temperature="28";			#不高于这个摄氏温度
	temperature=$(cat ${bindir}/data/wind.data 2> ${null});				#温度,摄氏度
	start_wind="${bindir}/info_light ${wiringpi_wind} 1";		#${wiringpi_wind} 高电压
	stop_wind="${bindir}/info_light ${wiringpi_wind} 0";		#${wiringpi_wind} 低电压
	status_wind="$(${bindir}/info_pin ${wiringpi_wind})";		: PIN状态.
	
}

#红外线守护进程.
function infrared()
{
    while true; do
        ${bindir}/info_infrared ${wiringpi_infrared} > ${bindir}/data/infrared.data;
        SLEEP 1;
    done
}

#灯主进程.
function light()
{
while true; do
    if test ${W} != "0"; then                           : 不是周日.
        if [ $now -ge $high -a $now -le $low ]; then	: 19:30 - 23:00自动检测,其他时间段不管,手动.
            if [ "${phonelogic}" == "0" ]; then			: phone 如果在线.
                if test "${lightpinvalue}" = "0"; then	: 检测灯pin值.
                    ${start_light};
                fi
            else
                if test "${lightpinvalue}" = "1"; then
                    ${stop_light};
                fi
            fi
        else
            if [ ${now} -gt ${high} ]; then             : 如果大于23点就关灯,不管,手动.
                if test "${lightpinvalue}" = "1"; then
                    ${stop_light};
                fi
            fi
        fi
    else	
        : 还一个周日
        if test "$W" = "0"; then								: 是周日.
            if [ ${now} -le ${low} -a ${now} -ge 0800 ]; then	: 8:00 - 23:00,自动化.
                if [ "${phonelogic}" == "0" ]; then				: phone 如果在线.
				    if test "${lightpinvalue}" = "0"; then		: 检测灯pin值.
					    ${start_light};
                    fi
                else
                    if test "${lightpinvalue}" = "1"; then
                        ${stop_light};
                    fi
                fi
            else
                if [ "${phonelogic}" == "1" ]; then             : 不在时间段就关闭,手动.
                    ${stop_light};
                fi
            fi
        fi				
    fi	

INIT;
SLEEP 60;
done
}

#声音传感器守护进程.
function sound_daemon()
{
    while :; do
        ${bindir}/info_sound ${wiringpi_sound} > ${bindir}/data/sound.data
        SLEEP 0.3;
    done
}

#声音传感器主进程.
function sound()
{
    :
}

#风扇守护进程.
function wind_daemon()
{
	while true; do
		${bindir}/wind > ${bindir}/data/wind.data;		: 温度传感器获取室内温度信息.
		SLEEP 9;
	done
}

#风扇主进程.
function wind()
{
INIT
    while true ; do
        if [[ "${phonelogic}" == "0" ]]; then	: 手机在不在?
			if [[ "${temperature}" -ge "${l_temperature}" ]]; then	: 温度大于等于$l_temperature摄氏度.
				if test "${status_wind}" = "0"; then
					${start_wind};
					SLEEP 180;		: 开启后风扇运行3分钟.
					${stop_wind};
					SLEEP 60;		: 开启后风扇停止运行1分钟,这样是怕吹感冒了.
				fi
			else
				test "${status_wind}" = "1" && ${stop_wind};		: 小于$l_temperature温度就检测或关闭.
			fi
		else
			test "${status_wind}" = "1" && ${stop_wind};		: 手机不再就检测或关闭.
		fi
		SLEEP 7;
		INIT;
    done
}

#主进程
function main_()
{
INIT;
while getopts :ds:x ai
do
case ${ai} in
    d)
        daemon='&';
        ;;
    s)
        case $OPTARG in
            "stop")
                array=$(ls ${bindir}/log/)
                for arr in ${array[@]}; do
                    kill $(cat ${bindir}/log/${arr}) &> ${null}
                    rm ${bindir}/log/${arr} 2> ${null}
                done
                exit 0
                ;;
            *)
                :
                ;;
        esac
        exit 0
        ;;
    x)
        set -x
        ;;
    h|?)
        echo -ne "
\e[1;31mraspberrypi zero w & 3, GPIO Modular\e[0m
$0 Usage: $0 [-?|h] [-d] [-s] [-x] [light|temperature|wind]
    -?|h : Printf Help
    -d   : Daemon.
    -s   : kill Daemon, signal: stop.
    -x   : Print commands and their arguments as they are executed.

    light       : light Modular.
    temperature : temperature Modular.
    wind        : Wind Modular.

by aixiao@aixiao.me.
"
exit 0
        ;;
esac
done

shift $((OPTIND-1))
#array=($(echo ${@}))
eval array=(${@})
for i in ${array[@]}; do
case $i in
    light)
        eval light ${daemon}
        echo $! > ${bindir}/log/light.pid 2> ${null}
        ;;
    temperature)
        #get18b20data &
        #echo $! > log/temperature.pid 2> ${null}
        :
        ;;
    sound)
        sound_daemon &
        echo $! > ${bindir}/log/sound.pid 2> ${null};
        eval sound ${daemon};
        ;;
    wind)
        wind_daemon &
        echo $! > ${bindir}/log/wind.pid 2> ${null}
        eval wind ${daemon}
        echo $! > ${bindir}/log/main.pid 2> ${null}
        ;;
    *)
        exit 1;
        ;;
esac
done
}

main_ $@


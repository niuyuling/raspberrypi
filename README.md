# raspberrypi

raspberrypi zero w GPIO 实现的智能家居
- 参考:http://shumeipai.nxez.com/2016/06/26/raspberry-pi-diy-with-a-smart-home-server.html

- 我发现传感器的数据是实时的,传感器的工作必须是实时的
- 有时做不到数据的准确
- 就做了多进程的shell(不同pid)来满足传感器数据现状,一个进程收集传感器数据,另一个进程处理数据.


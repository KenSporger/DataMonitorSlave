# DataMonitorSlave

## 简介

这是杭州电子科技大学电子信息学院专业综合实践设计课程作业“防火防盗监测器”项目的STM32下位机部分，主要功能有：

+ 实时温度监测；
+ 加速度计、陀螺仪传感数据获取；
+ 姿态角解算；
+ 数据曲线绘制；
+ 无线通信和数据上报；
+ 温度和震动报警；

上位机项目地址：https://github.com/KenSporger/DataMonitorHost



## 硬件平台

+ STM32F407开发板
+ MPU6050陀螺仪模块
+ DS18B20温度传感器模块
+ ESP8266-01 WIFI模块
+ HC-05蓝牙模块
+ 0.96寸OLED单色显示屏



## 软件环境

+ FreeRTOS
+ MDK-ARM V5



## 效果展示

![slave](https://gitee.com/KenSporger/typera-picgo/raw/master/img/slave.png)





## 快速使用

1. 配置WIFI、OLED、陀螺仪/加速度计、温度传感器、按键、蜂鸣器、数码管模块相应引脚。

2. 配置`wifi.c`文件中的WIFI名称和密码、服务器地址和端口。

   ```
   //wifi帐号和密码
    const uint8_t wifi_name[20]	="TP-LINK_DBAF";
    const uint8_t wifi_pass[20]	="wan666xue";
    
   //无线运动传感器节点服务器ip：
    const uint8_t wifi_ip[32]	="192.168.0.113";
    const uint8_t wifi_port[6]	="10456";
   ```

3. 编译下载至开发板。
4. 使用K1和K4键上下切换选项卡。
5. 在“实时监测”界面，使用K2和K3键切换温度/加速度/角速度页面。
6. 在“数据曲线”界面，使用K2和K3键切换温度/姿态角页面。
7. 在“无线通信”界面，可以查看WIFI连接情况（“OK”表示正常），使用K2和K3键开启/关闭数据上报。
8. 在“参数设置”界面，使用K5键切换参数选项卡，使用K2和K3键调节数值。







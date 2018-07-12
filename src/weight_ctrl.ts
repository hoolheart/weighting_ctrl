<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>CalibrationDlg</name>
    <message>
        <location filename="calibration_dlg.ui" line="14"/>
        <location filename="calibration_dlg.ui" line="93"/>
        <source>Calibration</source>
        <translation>重量标定</translation>
    </message>
    <message>
        <location filename="calibration_dlg.ui" line="22"/>
        <source>Calibration Value</source>
        <translation>当前标定的载荷值</translation>
    </message>
    <message>
        <location filename="calibration_dlg.ui" line="29"/>
        <source>g</source>
        <translation></translation>
    </message>
    <message>
        <location filename="calibration_dlg.ui" line="45"/>
        <source>Current AD Value</source>
        <translation>当前 AD 采样值</translation>
    </message>
    <message>
        <location filename="calibration_dlg.ui" line="59"/>
        <source>Calibration Point</source>
        <translation>选择标定点</translation>
    </message>
</context>
<context>
    <name>ConfigDlg</name>
    <message>
        <location filename="config_dlg.ui" line="14"/>
        <source>Configuration</source>
        <translation>串口配置</translation>
    </message>
    <message>
        <location filename="config_dlg.ui" line="22"/>
        <source>Baud Rate</source>
        <translation>波特率</translation>
    </message>
    <message>
        <location filename="config_dlg.ui" line="32"/>
        <source>Parity</source>
        <translation>校验位</translation>
    </message>
    <message>
        <location filename="config_dlg.ui" line="42"/>
        <source>Data Bit Number</source>
        <translation>数据位</translation>
    </message>
    <message>
        <location filename="config_dlg.ui" line="52"/>
        <source>Stop Bit Number</source>
        <translation>停止位</translation>
    </message>
    <message>
        <location filename="config_dlg.ui" line="66"/>
        <source>Confirm</source>
        <translation>确定</translation>
    </message>
    <message>
        <location filename="config_dlg.ui" line="86"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="config_dlg.cpp" line="68"/>
        <source>Failed to configure:
%1</source>
        <translation>配置串口失败：
%1</translation>
    </message>
</context>
<context>
    <name>SerialInterface</name>
    <message>
        <location filename="serial_interface.cpp" line="32"/>
        <source>The port can&apos;t be configured while it is open</source>
        <translation>端口已经打开，不能配置</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="79"/>
        <source>Serial port failed to open with no error</source>
        <translation>串口无法打开，但没有错误返回</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="95"/>
        <source>Failed to close serial port</source>
        <translation>无法关闭串口</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="113"/>
        <source>Failed to write command via serial port</source>
        <translation>通过串口发送命令失败</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="122"/>
        <source>The serial port is now closed</source>
        <translation>串口处于关闭状态</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="127"/>
        <source>Input command is invalid</source>
        <translation>输入的命令无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="154"/>
        <source>no parity</source>
        <translation>无校验</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="156"/>
        <source>even parity</source>
        <translation>偶校验</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="158"/>
        <source>odd parity</source>
        <translation>奇校验</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="160"/>
        <source>space parity</source>
        <translation>0校验</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="162"/>
        <source>mark parity</source>
        <translation>1校验</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="164"/>
        <source>Invalid parity setting</source>
        <translation>无效的校验设置</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="178"/>
        <source>Invalid stop bit setting</source>
        <translation>无效的停止位设置</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="274"/>
        <source>Bytes written to serial port are more than sent commands</source>
        <translation>从串口发送出的数据长度超过了应该发出的命令</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="279"/>
        <source>Too many pending bytes (%1) to write, the serial port may has been down</source>
        <translation>未发送的字节过多（%1），串口可能已经中断</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="297"/>
        <source>The port %1 is not available</source>
        <translation>串口（%1）无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="310"/>
        <source>The baud rate %1 is not a standard baud rate</source>
        <translation>波特率设置（%1）无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="315"/>
        <source>Invalid setting to parity</source>
        <translation>校验设置无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="320"/>
        <source>Invalid setting to data bits</source>
        <translation>数据位设置无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="325"/>
        <source>Invalid setting to stop bits</source>
        <translation>停止位设置无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="336"/>
        <source>No error</source>
        <translation>无错误</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="338"/>
        <source>Serial port is not found</source>
        <translation>串口未被找到</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="340"/>
        <source>Serial port has been used or user don&apos;t has enough permission</source>
        <translation>串口被占用，或用户没有足够的权限</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="342"/>
        <source>Failed to open serial port</source>
        <translation>无法打开串口</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="344"/>
        <source>Serial port has not been open</source>
        <translation>串口还未被打开</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="346"/>
        <source>An I/O error occurred while writing the data</source>
        <translation>在写数据过程中出现 IO 错误</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="348"/>
        <source>An I/O error occurred while reading the data</source>
        <translation>在读数据过程中出现 IO 错误</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="350"/>
        <source>Serial port becomes unavailable</source>
        <translation>串口变为无效</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="352"/>
        <source>The operation is not supported</source>
        <translation>不支持的操作</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="354"/>
        <source>The operation times out</source>
        <translation>操作超时</translation>
    </message>
    <message>
        <location filename="serial_interface.cpp" line="358"/>
        <source>Unknown or obsolete error code %1</source>
        <translation>未知的错误代码：%1</translation>
    </message>
</context>
<context>
    <name>WeightCtrlDlg</name>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="14"/>
        <source>Digital Pressure Sensor Calibration System</source>
        <translation>数字压力传感器标定系统</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="30"/>
        <source>WEIGHT (unit: g)</source>
        <translation>重量（单位：g）</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="68"/>
        <source>AD VALUE</source>
        <translation>AD 采样值</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="106"/>
        <source>NODE INDEX</source>
        <translation>反馈节点</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="160"/>
        <source>Calibrate</source>
        <translation>多段标定</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="170"/>
        <source>Reset</source>
        <translation>置零</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="193"/>
        <source>Help</source>
        <translation>帮助</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="203"/>
        <source>Cont. Read</source>
        <translation>循环读数</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="213"/>
        <source>Config</source>
        <translation>配置</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="252"/>
        <location filename="weight_ctrl_dlg.cpp" line="104"/>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="262"/>
        <source>Read</source>
        <translation>读数</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="272"/>
        <source>Port Select</source>
        <translation>串口选择</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="279"/>
        <source>Set</source>
        <translation>设置节点</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="289"/>
        <source>Node Select</source>
        <translation>节点选择</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="296"/>
        <source>Port Status</source>
        <translation>串口状态</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.ui" line="352"/>
        <source>Stop Read</source>
        <translation>停止读数</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="31"/>
        <source>No available serial port</source>
        <translation>无有效串口</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="95"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="118"/>
        <source>Failed to configure:
%1</source>
        <translation>配置串口失败：
%1</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="152"/>
        <source>Failed to control serial port:
%1</source>
        <translation>控制串口失败：
%1</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="169"/>
        <source>Failed to set device node:
%1</source>
        <translation>设置节点失败：
%1</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="180"/>
        <location filename="weight_ctrl_dlg.cpp" line="202"/>
        <source>Failed to read data:
%1</source>
        <translation>读数失败：
%1</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="217"/>
        <source>Failed to reset data:
%1</source>
        <translation>置零失败：
%1</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="245"/>
        <source>Port unspecified</source>
        <translation>未指定有效串口</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="249"/>
        <source>%1, %2, baudrate: %3, data bit: %4, parity: %5, stop bit: %6</source>
        <translation>%1，%2，波特率：%3，数据位：%4，%5，停止位：%6</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="250"/>
        <source>on</source>
        <translation>已打开</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="250"/>
        <source>off</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="287"/>
        <source>Failed to set node index</source>
        <translation>设置节点失败</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="290"/>
        <source>Failed to reset data</source>
        <translation>置零失败</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="293"/>
        <source>Failed to calibrate (point: %1)</source>
        <translation>标定失败（标定点：%1）</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="296"/>
        <source>Invalid code 0x%1 from node %2</source>
        <translation>节点号%2的设备反馈的无效功能号0x%1</translation>
    </message>
    <message>
        <location filename="weight_ctrl_dlg.cpp" line="310"/>
        <source>Failed to calibrate:
%1</source>
        <translation>标定失败：
%1</translation>
    </message>
</context>
</TS>

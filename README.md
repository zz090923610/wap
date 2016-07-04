wap
===

编译依赖包：

    cd 到 wap 目录
    sudo ./init.sh 

这一操作会通过apt-get 渠道安装需要的软件包，同时还有另外两个需要编译安装的依赖，
分别为 gsl 和 opencv。
理论上讲该脚本可以自动下载并安装 gsl 和 opencv 的源码包到 dl 目录内，
如果下载十分缓慢，可自行下载以下两个文件并存放到 dl目录内再开始./init.sh
理论上讲重新运行 init.sh 时，安装过的软件包就不需要再重新安装。
如果需要强制重新安装所有软件包，可以加 force 参数。

    sudo ./init.sh force

d8f70abafd3e9f0bae03c52d1f4e8de5 

ftp://ftp.gnu.org/gnu/gsl/gsl-2.1.tar.gz

7f958389e71c77abdf5efe1da988b80c 

http://nbtelecom.dl.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.9/opencv-2.4.9.zip

使用：

由于历史原因最终出来的可执行程序名称为 mapTest，源文件使用 cmake 管理，
为简化操作，写了一个 build.sh 脚本。该脚本实现了 configure, build, clean, 
demo 四个功能。

demo:
首次使用可先利用demo功能检测是否能正常运行。
    
    ./build.sh demo

该操作会执行一次 configure, build 操作，生成 mapTest 二进制程序，
然后使用 data/demo 目录里的一组样例数据执行它。

configure:

    ./build.sh configure

该操作应该在首次编译前和CMakeLists.txt文件被修改之后进行，以生成合适的 Makefile。

build:

    ./build.sh
    
编译操作，生成可执行的 mapTest 程序。生成程序会位于当前目录。

clean:

    ./build.sh clean

删除所有生成的中间文件以及 mapTest 程序。如果多次编译后出现问题，
可尝试先 clean，再重头 configure 以及 build。


mapTest 程序运行：

    ./mapTest path_to_sensor_data path_to_wifi_data path_to_start_point
    
这一程序需要传入三个参数文件，第一个参数是传感器数据的路径，
第二个参数是 wifi 数据的路径，第三个参数是起始位置和方向在地图坐标下的表示。
B315 从空调位置开始往西出发的参数是 1480 2580 180，
例如，实际上 demo 所做的事情就是 
    
    ./mapTest ./data/demo/Sensor0715122921.txt ./data/demo/Wifi0715122921.txt ./data/demo/start.txt
    
程序运行时会从特定目录读取所需的设置参数。这个目录是 ./params，
该目录里存放了除传感器数据、wifi数据和起点信息之外的其他所有参数。
一共有 config、allowedList.txt、mapData.txt、roomList.txt这四个文本文件。

config 文件为 UCI 格式的纯文本配置文件。关于 UCI 的具体使用方法请参阅 https://wiki.openwrt.org/doc/uci， 
其中包括 vars 和 paths 两组参数，
vars 这组参数中定义了粒子滤波用到的粒子数上限，生成粒子的半径，地图的缩放比例，
wifi进门检测时的作用阈值和wifi进门检测的延迟时间。
paths 这组参数制定了其他必要的三个地图相关数据文件的路径，
其中 map_file_path 为地图数据， room_list_path 为房间信息， 
allowed_list_path 为参与计算的AP列表。

关于地图相关数据。

mapData.txt 文件：

进行室内定位并进行粒子滤波，首先要有地图数据。程序运行时的二维地图在内部的表示是一个二位数组，
其中可以通过的无障碍的地方赋一种值，不可通过的地方（墙）赋另一种值。
这个数组中，左上角代表西北方向，右下角代表东南方向。(0, 0)点代表地图空间的最西北角。
(400,800)点代表的位置是(0,0) 点以南400,以东800的位置。其中 1 单位代表 1CM。

而 mapData.txt 这个文件存放了地图里所有墙的数据，每堵墙都是一个矩形，
每一行存放一堵墙的信息，墙用一个四元组定义，
其中四个值分别代表这个墙的矩形的西北角的点的坐标和东南角的点的坐标，均为正整数，
用空格和回车隔开。
比如第一行：
    
    0	0	240	66370

就代表南楼最北边的一堵外墙。 0 0 代表这堵墙的西北点，240 66370 代表这堵墙的东南点的坐标。

除了用于内部实际运算，这个文件还用于绘制地图。config 中的 zoom_scale 这个缩放比例就与此相关。

roomList.txt 文件：

这个文件用于存放地图上具体房间的信息，每行一组数据代表一个房间，用空格和回车隔开。
每行的数据分别为 房间名， 房间内的AP的MAC地址（BSSID）如没有则为 NULL，
房间所形成矩形的北南东西值，这个房间门形成的矩形的西东北南值。

该文件同样用于内部实际运算和绘制地图。

allowedList.txt 文件：

这个文件用于存放被用来参与计算的 AP 的信息。因为手机搜到的 AP 信息有很多，
我们应该只从中筛选出在我们地图上有信息的 AP 进行计算。
同样一行一个 AP，用空格和回车隔开。

每行的数据为该 AP 的 BSSID（即MAC地址），该AP 在地图上的坐标，
该 AP 的 参考 RSSI 和 gamma 值。
其中有效数据只有前三个，参与计算以及地图绘制， 参考 RSSI 和 gamma 为历史遗留数据，
并不参与任何计算过程。


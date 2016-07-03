wap
===

编译依赖包：

    cd 到 wap 目录
    sudo ./init.sh 

这一操作会通过apt-get 渠道安装需要的软件包，同时还有另外两个需要编译安装的依赖，分别为 gsl 和 opencv。
理论上讲该脚本可以自动下载并安装 gsl 和 opencv 的源码包到 dl 目录内，如果下载十分缓慢，可自行下载以下两个文件并存放到 dl目录内再开始./init.sh
理论上讲重新运行 init.sh 时，安装过的软件包就不需要再重新安装。如果需要强制重新安装所有软件包，可以加 force 参数。

    sudo ./init.sh force

d8f70abafd3e9f0bae03c52d1f4e8de5 ftp://ftp.gnu.org/gnu/gsl/gsl-2.1.tar.gz
7f958389e71c77abdf5efe1da988b80c http://nbtelecom.dl.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.9/opencv-2.4.9.zip

使用：

由于历史原因最终出来的可执行程序名称为 mapTest，源文件使用 cmake 管理，为简化操作，写了一个 build.sh 脚本。该脚本实现了 configure, build, clean, demo 四个功能。

demo:
首次使用可先利用demo功能检测是否能正常运行。
    
    ./build demo

该操作会执行一次 configure, build 操作，生成 mapTest 二进制程序，然后使用 data/demo 目录里的一组样例数据执行它。

configure:

    ./build configure

该操作应该在首次编译前和CMakeLists.txt文件被修改之后进行，以生成合适的 Makefile。

build:

    ./build
    
编译操作，生成可执行的 mapTest 程序。生成程序会位于当前目录。

clean:

    ./build clean

删除所有生成的中间文件以及 mapTest 程序。如果多次编译后出现问题，可尝试先 clean，再重头 configure 以及 build。


mapTest 程序运行：

    ./mapTest path_to_sensor_data path_to_wifi_data path_to_start_point
    
这一程序需要传入三个参数文件，第一个参数是传感器数据的路径，第二个参数是 wifi 数据的路径，第三个参数是起始位置和方向在地图坐标下的表示。315从空调位置开始往西出发的参数是 1480 2580 180，
例如，实际上 demo 所做的事情就是 
    
    ./mapTest ./data/demo/Sensor0715122921.txt ./data/demo/Wifi0715122921.txt ./data/demo/start.txt
    

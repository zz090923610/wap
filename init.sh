#!/bin/bash


install_dep_pkg ()
{
	# $1 archive type
	# $2 pkg file name
	# $3 pkg file md5
	# $4 pkg download url
	# $5 pkg configure type
	# $6 checkout val for git
	# $7 cmake fix cmd

	# download
	archive_type="$1"
	pkg_file="$2"
	md5=`[ -f "$pkg_file" ] && md5sum "$pkg_file" | grep -o '[a-z0-9]* ' | tr -d ' '`
	if [ $archive_type = 'git' ]
	then
        if [ $md5 = "$3" ]
            then
                echo -n '' # do nothing
            else
                name=`echo -n "$2" | grep -o "^[-_a-zA-Z0-9]*." | tr -d '.'`
                rm -rf "$name"
                git clone "$4"
                cd "$name"
                git checkout "$6"
                rm -rf .git
                cd ..
                tar -cvf "$2" "$name"
                chmod 777 "$2"
                rm -rf "$name"
        fi
	else
        if [ $md5 = "$3" ]
        then
            echo -n '' # do nothing
        else
            rm -f "$pkg_file"
            wget -c "$4" && chmod 777 "$pkg_file"
        fi
	fi
	# extract
	rm -rf tmp_dir 2>/dev/null
	[ "$archive_type" = "tar.gz" -o "$archive_type" = "git" ] && mkdir -p tmp_dir && chmod 777 tmp_dir && tar -xf "$pkg_file" -C tmp_dir
	[ "$archive_type" = "zip" ] && mkdir -p tmp_dir && unzip "$pkg_file" -d tmp_dir

	# make && install
	[ "$5" = "gnumake" ] && cd tmp_dir/`ls tmp_dir` && ./configure && make && sudo make install && cd ../.. && rm -rf tmp_dir 2>/dev/null
	[ "$5" = "cmake" ] && cd tmp_dir/`ls tmp_dir` && cmake -G "Unix Makefiles" && make && sudo make install && cd ../.. && rm -rf tmp_dir 2>/dev/null
	[ "$5" = "cmake-fix" ] && (cd tmp_dir/`ls tmp_dir` && $($7); cmake -G "Unix Makefiles" && make && sudo make install && cd ../.. && rm -rf tmp_dir 2>/dev/null)
	[ "$5" = "cmake-post-fix" ] && (cd tmp_dir/`ls tmp_dir` && cmake -G "Unix Makefiles" && make && sudo make install && $($7); cd ../.. && rm -rf tmp_dir 2>/dev/null)
	[ "$5" = "make_linux" ] && cd tmp_dir/`ls tmp_dir` && make linux && sudo make install && cd ../.. && rm -rf tmp_dir 2>/dev/null
}




# start here
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

sudo apt-get install build-essential libncurses5-dev zlib1g-dev gawk gcc-multilib flex git gettext cmake libgtk2.0-dev unzip tar libreadline6-dev
mkdir -p ./dl && chmod 777 ./dl
cd dl
rm -rf tmp_dir 2>/dev/null

echo -n install lua... && [ `ls /usr/local/lib | grep lua | wc -l` = '0' -o "$1" = force ] && install_dep_pkg tar.gz lua-5.1.5.tar.gz 2e115fe26e435e33b0d5c022e4490567 http://www.lua.org/ftp/lua-5.1.5.tar.gz make_linux; echo [OK]
echo -n install ubox... && [ `ls /usr/local/lib | grep ubox | wc -l` = '0' -o "$1" = force ] && install_dep_pkg git libubox.tar.gz 3a98c87d3eac015724239f2ea7acfffb http://git.openwrt.org/project/libubox.git cmake-fix 5326ce1046425154ab715387949728cfb09f4083 "sed -i s/ADD_SUBDIRECTORY(examples)//g CMakeLists.txt"; echo [OK]
echo -n install uci... && [ `ls /usr/local/lib | grep uci | wc -l` = '0' -o "$1" = force ] && install_dep_pkg git uci.tar.gz 3b74685583fa23094f6142f645cfd41b http://git.openwrt.org/project/uci.git cmake-post-fix d4e42a9081e0b6fa5cbb072bcbfe01adccdad694 "sudo ln -s /usr/local/lib/libubox.so /usr/local/lib/libuci.so /usr/lib/"; echo [OK]
echo -n install gsl... && [ `ls /usr/lib | grep gsl | wc -l` = '0' -o "$1" = force ] && install_dep_pkg tar.gz gsl-2.1.tar.gz d8f70abafd3e9f0bae03c52d1f4e8de5 ftp://ftp.gnu.org/gnu/gsl/gsl-2.1.tar.gz gnumake; echo [OK]
echo -n install opencv... && [ `ls /usr/local/lib | grep opencv | wc -l` = '0' -o "$1" = force ] && install_dep_pkg zip opencv-2.4.9.zip  7f958389e71c77abdf5efe1da988b80c http://nbtelecom.dl.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.9/opencv-2.4.9.zip cmake; echo [OK]
cd ..


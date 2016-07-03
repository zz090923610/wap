#!/bin/bash


install_dep_pkg ()
{
	# $1 archive type
	# $2 pkg file name
	# $3 pkg file md5
	# $4 pkg download url
	# $5 pkg configure type
	archive_type="$1"
	pkg_file="$2"
	md5=`[ -f "$pkg_file" ] && md5sum "$pkg_file" | grep -o '[a-z0-9]* ' | tr -d ' '`
	if [ $md5 = "$3" ]
	then
		echo -n '' # do nothing
	else
		rm -f "$pkg_file"
		wget -c "$4" && chmod 777 "$pkg_file"
	fi
	rm -rf tmp_dir 2>/dev/null
	[ "$archive_type" = "tar.gz" ] && mkdir -p tmp_dir && chmod 777 tmp_dir && tar -xf "$pkg_file" -C tmp_dir
	[ "$archive_type" = "zip" ] && mkdir -p tmp_dir && unzip "$pkg_file" -d tmp_dir
	[ "$5" = "gnumake" ] && cd tmp_dir/`ls tmp_dir` && ./configure && make && sudo make install && cd ../.. && rm -rf tmp_dir 2>/dev/null
	[ "$5" = "cmake" ] && cd tmp_dir/`ls tmp_dir` && cmake -G "Unix Makefiles" && make && sudo make install && cd ../.. && rm -rf tmp_dir 2>/dev/null
}




# start here

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

sudo apt-get install build-essential libncurses5-dev zlib1g-dev gawk gcc-multilib flex git gettext cmake libgtk2.0-dev unzip tar
mkdir -p ./dl && chmod 777 ./dl
cd dl
rm -rf tmp_dir 2>/dev/null
install_dep_pkg tar.gz gsl-2.1.tar.gz d8f70abafd3e9f0bae03c52d1f4e8de5 ftp://ftp.gnu.org/gnu/gsl/gsl-2.1.tar.gz gnumake
install_dep_pkg zip opencv-2.4.9.zip  7f958389e71c77abdf5efe1da988b80c http://nbtelecom.dl.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.9/opencv-2.4.9.zip cmake
cd ..


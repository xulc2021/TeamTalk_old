#!/bin/bash

CUR_DIR=$(pwd)

# 编译base库
build_base() {
	cd $CUR_DIR
	if [ ! -d "lib/base/" ]; then
		mkdir "lib/base" -p
	fi

	# 编译base库
	cd "lib/base"
	cmake ../../base
	make
	if [ $? -eq 0 ]; then
		echo "make base successed"
	else
		echo "make base failed"
		exit;
	fi

	cp libbase.a ../
	cp -a liblog4cxx.so* ../
}

# 编译log库（依赖的log4cxx通过make_log4cxxx.sh到../lib得到）
build_slog(){
	cd $CUR_DIR
	if [ ! -d "lib/slog/" ]; then
		mkdir "lib/slog" -p
	fi
	cd "lib/slog"
	cmake ../../slog
	make 
	if [ $? -eq 0 ]; then
		echo "make slog successed"
	else
		echo "make slog failed"
		exit
	fi

	cp libslog.dylib ../
}

# 编译login_server（依赖base,slog库）
build_login_server(){
	cd $CUR_DIR
	if [ ! -d "bin/login_server/" ]; then
		mkdir "bin/login_server" -p
	fi
	cd "bin/login_server"
	cmake ../../login_server
	make
	if [ $? -eq 0 ]; then
		echo "make login_server successed"
	else
		echo "make login_server failed"
		exit
	fi

	# 拷贝配置文件
	cp ../../login_server/log4cxx.properties ./ & cp ../../login_server/loginserver.conf ./
}

# 编译 route_server（依赖base,slog库）
build_route_server(){
	cd $CUR_DIR
	if [ ! -d "bin/route_server/" ]; then
    	mkdir "bin/route_server" -p
	fi
	cd "bin/route_server"
	cmake ../../route_server
	make
	if [ $? -eq 0 ]; then
		echo "make route_server successed"
	else
		echo "make route_server failed"
	    exit
	fi

	# 拷贝配置文件
	cp ../../route_server/log4cxx.properties ./ & cp ../../route_server/routeserver.conf ./
}

# 编译 msg_server（依赖base,slog库）
build_msg_server(){
	cd $CUR_DIR
	if [ ! -d "bin/msg_server/" ]; then
    	mkdir "bin/msg_server" -p
	fi
	cd "bin/msg_server"
	cmake ../../msg_server
	make
	if [ $? -eq 0 ]; then
		echo "make msg_server successed"
	else
		echo "make msg_server failed"
	    exit
	fi

	# 拷贝配置文件
	cp ../../msg_server/log4cxx.properties ./ & cp ../../msg_server/msgserver.conf ./
}

# 编译 http_msg_server（依赖base,slog库）
build_http_msg_server(){
	cd $CUR_DIR
	if [ ! -d "bin/http_msg_server/" ]; then
    	mkdir "bin/http_msg_server" -p
	fi
	cd "bin/http_msg_server"
	cmake ../../http_msg_server
	make
	if [ $? -eq 0 ]; then
		echo "make http_msg_server successed"
	else
		echo "make http_msg_server failed"
	    exit
	fi
	# 拷贝配置文件
	cp ../../http_msg_server/log4cxx.properties ./ & cp ../../http_msg_server/httpmsgserver.conf ./
}

# 编译 file_server（依赖base,slog库）
build_file_server(){
	cd $CUR_DIR
	if [ ! -d "bin/file_server/" ]; then
    	mkdir "bin/file_server" -p
	fi
	cd "bin/file_server"
	cmake ../../file_server
	make
	if [ $? -eq 0 ]; then
		echo "make file_server successed"
	else
		echo "make file_server failed"
	    exit
	fi
	# 拷贝配置文件
	cp ../../file_server/log4cxx.properties ./ & cp ../../file_server/fileserver.conf ./
}

# 编译 push_server（依赖slog库）
build_push_server(){
	cd $CUR_DIR
	if [ ! -d "bin/push_server/" ]; then
    	mkdir "bin/push_server" -p
	fi
	cd "bin/push_server"
	cmake ../../push_server
	make
	if [ $? -eq 0 ]; then
		echo "make push_server successed"
	else
		echo "make push_server failed"
	    exit
	fi
	# 拷贝配置文件
	cp ../../push_server/log4cxx.properties ./ & cp ../../push_server/pushserver.conf ./
}

# 编译 db_proxy_server（依赖base,slog）
build_db_proxy_server(){
	cd $CUR_DIR
	if [ ! -d "bin/db_proxy_server/" ]; then
    	mkdir "bin/db_proxy_server" -p
	fi
	cd "bin/db_proxy_server"
	cmake ../../db_proxy_server
	make
	if [ $? -eq 0 ]; then
		echo "make db_proxy_server successed"
	else
		echo "make db_proxy_server failed"
	    exit
	fi
	# 拷贝配置文件
	cp ../../db_proxy_server/log4cxx.properties ./ & cp ../../db_proxy_server/dbproxyserver.conf ./
}

# 编译 msfs（依赖base,slog）
build_msfs(){
	cd $CUR_DIR
	if [ ! -d "bin/msfs/" ]; then
    	mkdir "bin/msfs" -p
	fi
	cd "bin/msfs"
	cmake ../../msfs
	make
	if [ $? -eq 0 ]; then
		echo "make msfs successed"
	else
		echo "make msfs failed"
	    exit
	fi
	# 拷贝配置文件
	cp ../../msfs/log4cxx.properties ./ & cp ../../msfs/msfs.conf ./
}

# 编译websocket_server
build_websocket_server(){
	# 先编译websocket库
	cd $CUR_DIR
	if [ ! -d "lib/websocket/" ]; then
    	mkdir "lib/websocket" -p
	fi
	cd "lib/websocket"
	cmake ../../websocket_server/websocket
	make
	if [ $? -eq 0 ]; then
		echo "make websocket_server successed"
	else
		echo "make websocket_server failed"
	    exit
	fi
	cp libwebsocket.a ../

	# 再编译websocket_server
	cd $CUR_DIR
	if [ ! -d "bin/websocket_server/" ]; then
    	mkdir "bin/websocket_server" -p
	fi
	cd "bin/websocket_server"
	cmake ../../websocket_server
	make
	if [ $? -eq 0 ]; then
		echo "make websocket_server successed"
	else
		echo "make websocket_server failed"
	    exit
	fi
	# 拷贝配置文件
	cp ../../websocket_server/log4cxx.properties ./ & cp ../../websocket_server/websocket_server.conf ./
}

# 编译 守护程序
build_tools(){
	cd $CUR_DIR
	if [ ! -d "bin/tools/" ]; then
    	mkdir "bin/tools" -p
	fi
	cd "tools"
	make
	if [ $? -eq 0 ]; then
		echo "make tools successed"
	else
		echo "make tools failed"
	    exit
	fi
	# 拷贝配置文件
	cp daeml ../bin/tools
}

build_all(){
# 如果报错，请手动执行安装
#	brew -y install cmake
#    brew -y install libuuid-devel
#    brew -y install openssl-devel
#    brew -y install curl-devel

	echo "#ifndef __VERSION_H__" > base/version.h
	echo "#define __VERSION_H__" >> base/version.h
	echo "#define VERSION \"$1\"" >> base/version.h
	echo "#endif" >> base/version.h

	build_base
	build_slog
	build_login_server
	build_msg_server
	build_http_msg_server
	build_push_server
	build_db_proxy_server
	build_route_server
	#build_file_server
	build_msfs
	build_tools
	build_websocket_server

	cd $CUR_DIR/bin/
		
	#copy executables to run/ dir
	mkdir -p ../../run/login_server
    mkdir -p ../../run/route_server
    mkdir -p ../../run/msg_server
    mkdir -p ../../run/file_server
    mkdir -p ../../run/msfs
    mkdir -p ../../run/push_server
    mkdir -p ../../run/http_msg_server

	
    mkdir -p ../../run/db_proxy_server
    mkdir -p ../../run/websocket_server

	#copy executables to run/ dir
	cp login_server/login_server ../../run/login_server/

	cp route_server/route_server ../../run/route_server/

	cp msg_server/msg_server ../../run/msg_server/

    cp http_msg_server/http_msg_server ../../run/http_msg_server/

    cp file_server/file_server ../../run/file_server/

    cp push_server/push_server ../../run/push_server/

    cp db_proxy_server/db_proxy_server ../../run/db_proxy_server/

    cp msfs/msfs ../../run/msfs/

    cp websocket_server/websocket_server ../../run/websocket_server/

    cp tools/daeml ../../run/

    build_version=im-server-$1
    build_name=$build_version.tar.gz
	if [ -e "$build_name" ]; then
		rm $build_name
	fi
    mkdir -p ../../$build_version
    mkdir -p ../../$build_version/login_server
    mkdir -p ../../$build_version/route_server
    mkdir -p ../../$build_version/msg_server
    mkdir -p ../../$build_version/file_server
    mkdir -p ../../$build_version/msfs
    mkdir -p ../../$build_version/http_msg_server
    mkdir -p ../../$build_version/push_server

    mkdir -p ../../$build_version/db_proxy_server
    mkdir -p ../../$build_version/lib
    mkdir -p ../../$build_version/websocket_server

    cp login_server/loginserver.conf ../../$build_version/login_server/
    cp login_server/login_server ../../$build_version/login_server/

    cp route_server/route_server ../../$build_version/route_server/
    cp route_server/routeserver.conf ../../$build_version/route_server/

    cp msg_server/msg_server ../../$build_version/msg_server/
    cp msg_server/msgserver.conf ../../$build_version/msg_server/

    cp http_msg_server/http_msg_server ../../$build_version/http_msg_server/
    cp http_msg_server/httpmsgserver.conf ../../$build_version/http_msg_server/

    cp file_server/file_server ../../$build_version/file_server/
    cp file_server/fileserver.conf ../../$build_version/file_server/

    cp push_server/push_server ../../$build_version/push_server/
    cp push_server/pushserver.conf ../../$build_version/push_server/

   
    cp db_proxy_server/db_proxy_server ../../$build_version/db_proxy_server/
    cp db_proxy_server/dbproxyserver.conf ../../$build_version/db_proxy_server/

    cp msfs/msfs ../../$build_version/msfs/
    cp msfs/msfs.conf ../../$build_version/msfs/

    cp websocket_server/websocket_server ../../$build_version/websocket_server/
    cp websocket_server/websocket_server.conf ../../$build_version/websocket_server/

    cp ../slog/log4cxx.properties ../../$build_version/lib/
    cp ../lib/libslog.dylib  ../../$build_version/lib/
    cp -a ../lib/liblog4cxx* ../../$build_version/lib/
    cp ../sync_lib_for_zip.sh ../../$build_version/

    cp tools/daeml ../../$build_version/
    cp ../../run/restart.sh ../../$build_version/
    cp ../../run/stop.sh ../../$build_version/

    cd ../../
    tar zcvf    $build_name $build_version

    rm -rf $build_version
}

clean() {
	cd lib/base
	make clean
	cd lib/slog
	make clean
	cd $CUR_DIR/bin/login_server
	make clean
	cd $CUR_DIR/bin/route_server
	make clean
	cd $CUR_DIR/bin/msg_server
	make clean
	cd $CUR_DIR/bin/http_msg_server
    make clean
	cd $CUR_DIR/bin/file_server
    make clean
    cd $CUR_DIR/bin/push_server
    make clean
	cd $CUR_DIR/bin/db_proxy_server
	make clean
    cd $CUR_DIR/bin/push_server
    make clean
    cd $CUR_DIR/bin/websocket_server/websocket
    make clean
    cd $CUR_DIR/bin/websocket_server/websocket_server
    make clean
}

print_help() {
	echo "Usage: "
	echo "  $0 clean --- clean all build"
	echo "  $0 version version_str --- build a version"
}

case $1 in
	clean)
		echo "clean all build..."
		clean
		;;
	version)
		if [ $# != 2 ]; then 
			echo "build version:$#"
			print_help
			exit
		fi

		echo "ready build version:$2"
		echo "build..."
		build_all $2
		;;
	*)
		print_help
		;;
esac

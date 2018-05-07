#!/bin/bash

srcpaths=(slog base login_server db_proxy_server push_server route_server file_server msg_server msfs http_msg_server)
base_src_path=/opt/src
cd $base_src_path

for sub_src_path in ${srcpaths[@]}
do
    cd $sub_src_path
    cmake .
    make
    cd ..
done


mkdir -p /opt/im_server/lib
cp /opt/src/slog/libslog.* /opt/im_server/lib/

delete=(slog base)

servers=${array[@]/$delete}

for server in ${servers[@]}
do
    mkdir -p /opt/im_server/$server
    cp -f /opt/src/$server/$server /opt/im_server/$server/$server
done


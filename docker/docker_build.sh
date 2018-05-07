#!/bin/bash
srcpath=../server/src/
codepaths=(slog base login_server db_proxy_server push_server route_server file_server msg_server msfs http_msg_server)
mkdir src

for codepath in ${codepaths[@]}
do
    cp -rf $srcpath$codepath ./src/
done

cp -r ../pb ./src/
rm -rf ./src/base/pb/protocol/*


docker_image_name=xiaominfc/centos_tt_test
docker_file=./Dockerfile

docker kill $(docker ps -q)
docker build --rm -t $docker_image_name -f $docker_file . 

#docker run -d  --privileged=true  -v /sys/fs/cgroup:/sys/fs/cgroup $docker_image_name /usr/sbin/init
docker run -d  --privileged=true  -v /sys/fs/cgroup:/sys/fs/cgroup $docker_image_name
docker exec -it $(docker ps -q) /bin/bash


FROM centos:latest

RUN yum update;

#VOLUME ["/sys/fs/cgroup:/sys/fs/cgroup"]

#mariadb 
RUN yum -y install mariadb mariadb-server;
RUN systemctl enable mariadb;
#RUN systemctl start mariadb;
#RUN mysqladmin -u root password root;



#redis
RUN yum -y install epel-release;
RUN yum -y install redis;
RUN systemctl enable redis;
#RUN systemctl start redis;

#compile env
RUN yum -y install gcc gcc-c++ make cmake libuuid-devel openssl-devel curl-devel;
RUN yum -y install log4cxx-devel protobuf-lite-devel hiredis-devel mariadb-devel;
RUN yum -y install protobuf;

RUN yum clean all;


# cp source code
RUN mkdir -p /opt/src 

COPY ./src /opt/src/
COPY ./build.sh /opt/



RUN protoc -I=/opt/src/pb --cpp_out=/opt/src/base/pb/protocol/ /opt/src/pb/*.proto
RUN chmod +x /opt/build.sh

CMD ["/usr/sbin/init"]
RUN bash /opt/build.sh

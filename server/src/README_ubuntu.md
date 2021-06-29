# Build in Ubuntu

## proepare

```bash
$ chmod 777 *.sh
$ sudo ./make_hiredis.sh
$ sudo ./make_protobuf.sh
$ sudo apt install liblog4cxx-dev     # log4cxx
$ sudo apt install libmysqlclient-dev # mysql.h
```

## Base

```bash
# github/TeamTalk/server/src
$ cd base
$ cmake .
$ make
$ cp libbase.a ../lib
$ cd ..
```

## Slog

```bash
# github/TeamTalk/server/src
$ cd slog
$ cmake . && make
$ cp libslog.so ../lib
$ cd ..
```

## login_server

```bash
# github/TeamTalk/server/src
$ mkdir -p bin/login_server
$ cmake ../../login_server/
$ make
```

## msg_server

```bash
$ mkdir -p bin/msg_server
$ cmake ../../msg_server/
$ make
```

## route_server

```bash
$ mkdir -p bin/route_server
$ cmake ../../route_server/
$ make
```

## db_proxy_server

```bash
$ vim  db_proxy_server/CMakeList.txt
# SET(MYSQL_LIB_NAME mysqlclient_r)
SET(MYSQL_LIB_NAME mysqlclient) 

$ mkdir -p bin/db_proxy_server
$ cmake ../../db_proxy_server/
$ make
```
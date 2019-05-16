# RestAPI

1.获取低负载服务器地址
/msg_server 

请求:  
```json
{
}
```
响应:
```json
{
"code": 0,
"msg": "xxx",
"priorIP": "106.14.172.35",	//msg_server
"backupIP": "106.14.172.35",
"port": "9090",				// msg_server
"msfsPrior": "",			// msfs_server
"msfsBackup": "",				
"discovery": "",			// not use
}
```
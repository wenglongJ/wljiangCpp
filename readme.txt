1.安装ssh
2.安装protbuf
3.安装mysql ,mysql connect 
4.创建数据库和表 数据库SECMNG 表 KEYSN  , SECKEYINFO 
  KEYSN 
    ikeysn  created_time   updated_time    
  SECKEYINFO 
    id  clientid    serverid   keyid  createtime    state  seckey    created_time   updated_time   
5.建立build bin文件夹，在build目录下执行cmake ..  mk  之后在bin目录下执行 ./demo
        
使用脚本评测时需使用sudo命令：

```
sudo python2 autograde.py
```

如需使用系统保留的端口，也需要使用sudo命令(默认端口为21)：

```
sudo ./server
```

server没有任何标准输出，但在出现错误时可能会向stderr输出。
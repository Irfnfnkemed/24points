# 24points
This is a little practice based on Flask and Socket.

## use
Start service by running the script. Modify the port in the script if you need:
```
  ./run.sh
```

The website can be accessed within the local area network. You may need to turn off the firewall.


### marks
The following operations require administrator privileges.

If the process is running in WSL, port mapping is required. Run in `Powersell` with administrator privileges:
```
netsh interface portproxy add v4tov4 listenport=<target-port> listenaddress=0.0.0.0 connectport=<app-port> connectaddress=<wsl-ip>
```

Check the mapping by running:
```
netsh interface portproxy show all
```

Now the website can be visited by `<windows-ip>:<target-port>`.

If you want to delete the mapping, running:
```
netsh interface portproxy delete v4tov4 listenport=<target-port> listenaddress=0.0.0.0
```

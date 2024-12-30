# http shells

each http shell has a unique  rest endpoint that has as http method GET
```
/backdoor
```
This rest api has a query parameter called cmd. To call a http shell then you should do as the following:
```
http://vulnerablemachineip:3030/backdoor?cmd=command
```

where:
- vulnerablemachineip: is the ip where the web shell is installed.
- command: is the command given to the webshell

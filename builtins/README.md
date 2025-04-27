# Test cases to fix

## export
```
export A-
```
mini exit code = 0<br>
bash exit code = 1<br>
mini error = ()<br>
bash error = ( not a valid identifier)<br>
```
export HELLO-=123
```
mini exit code = 0<br>
bash exit code = 1<br>
mini error = ()<br>
bash error = ( not a valid identifier)<br>
```
export =
```
mini exit code = 0<br>
bash exit code = 1<br>
mini error = ()<br>
bash error = ( not a valid identifier)<br>
```
export 123
```
mini exit code = 0<br>
bash exit code = 1<br>
mini error = ()<br>
bash error = ( not a valid identifier)<br>

## unset
```
unset
```
mini exit code = 139<br>
bash exit code = 0<br>
```
unset HELLO
```
mini exit code = 139<br>
bash exit code = 0<br>
```
unset HELLO1 HELLO2
```
mini exit code = 139<br>
bash exit code = 0<br>

## pwd
```
cd $PWD hi
```
mini exit code = 0<br>
bash exit code = 1<br>
mini error = ()<br>
bash error = ( too many arguments)<br>
```
cd 123123
```
mini error = ()<br>
bash error = ( No such file or directory)<br>

## exit
```
exit 123
```
mini exit code = 0<br>
bash exit code = 123<br>
```
exit 298
```
mini exit code = 0<br>
bash exit code = 42<br>
```
exit +100
```
mini exit code = 0<br>
bash exit code = 100<br>
```
exit "+100"
```
mini exit code = 0<br>
bash exit code = 100<br>
```
exit +"100"
```
mini exit code = 0<br>
bash exit code = 100<br>
```
exit -100
```
mini exit code = 0
bash exit code = 156<br>
```
exit "-100"
```
mini exit code = 0
bash exit code = 156<br>
```
exit -"100"
```
mini exit code = 0
bash exit code = 156<br>
```
exit hello
```
mini exit code = 0<br>
bash exit code = 2<br>
mini error = ()<br>
bash error = ( numeric argument required)<br>
```
exit 42 world
```
mini exit code = 0<br>
bash exit code = 1<br>
mini error = ()<br>
bash error = ( too many arguments)<br>

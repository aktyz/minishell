# Test cases to fix

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

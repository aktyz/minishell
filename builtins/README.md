# Test cases to fix

## export
```
export A-
```
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( not a valid identifier)
```
export HELLO-=123
```
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( not a valid identifier)
```
export =
```
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( not a valid identifier)
```
export 123
```
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( not a valid identifier)

## unset
```
unset
```
mini exit code = 139
bash exit code = 0
```
unset HELLO
```
mini exit code = 139
bash exit code = 0
```
unset HELLO1 HELLO2
```
mini exit code = 139
bash exit code = 0

## pwd
```
cd $PWD hi
```
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( too many arguments)
```
cd 123123
```
mini error = ()
bash error = ( No such file or directory)

## exit
```
exit 123
```
mini exit code = 0
bash exit code = 123
```
exit 298
```
mini exit code = 0
bash exit code = 42
```
exit +100
```
mini exit code = 0
bash exit code = 100
```
exit "+100"
```
mini exit code = 0
bash exit code = 100
```
exit +"100"
```
mini exit code = 0
bash exit code = 100
```
exit -100
```
mini exit code = 0
bash exit code = 156
```
exit "-100"
```
mini exit code = 0
bash exit code = 156
```
exit -"100"
```
mini exit code = 0
bash exit code = 156
```
exit hello
```
mini exit code = 0
bash exit code = 2
mini error = ()
bash error = ( numeric argument required)
```
exit 42 world
```
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( too many arguments)

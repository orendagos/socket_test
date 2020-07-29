# socket_test
socket_test

## bug
1、client和server启动的时候，有时候client会接收到一个空的信息，ret==0，所以导致client退出
2、server端启动的时候，线程执行函数未进入，但是线程创建成功了，而且代码中加了pthread_join，概率性出现，待解决。

## 0729解决问题2
pthread_join的原型是：
int pthread_join(pthread_t thread, void **retval);
not ：int pthread_join(pthread_t* thread, void **retval);

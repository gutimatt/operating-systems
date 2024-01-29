LoadBalancer.c and InstanceHost.c are included
User.c and header files were not adjusted to run these

gcc *.c -o out -pthread
./out
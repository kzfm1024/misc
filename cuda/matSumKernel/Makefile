all: drivertest matSumKernel.ptx

drivertest:
	g++ -o drivertest drivertest.cpp -lcuda

matSumKernel.ptx: matSumKernel.cu
	nvcc matSumKernel.cu -ptx -o matSumKernel.ptx

run: all
	./drivertest

clean:
	rm -f drivertest matSumKernel.ptx *~

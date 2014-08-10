http://ceres-solver.org/building.html の Building on Linux を参考にして
ceres をインストール

$ sudo apt-get install libgflags-dev
$ sudo apt-get install libgoogle-glog-dev

$ sudo apt-get install libatlas-base-dev
$ sudo apt-get install libeigen3-dev
$ sudo apt-get install libsuitesparse-dev

$ git clone https://ceres-solver.googlesource.com/ceres-solver

$ mkdir ceres-bin
$ cd ceres-bin
$ cmake ../ceres-solver
$ make -j3

loss_function.cc に #include <limits> 追加
  
$ make test
$ sudo make install

----------------------------------------------------------------------

$ git clone https://github.com/snavely/bundler_sfm.git
$ cd bundler_sfm
$ make


$ sudo apt-get install jhead

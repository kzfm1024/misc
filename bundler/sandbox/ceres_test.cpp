// http://buaiso.blogspot.jp/2013/12/ceres-solver.html

#include <iostream>
#include "cvmat_util.h"
#include <ceres/ceres.h>

#if 0
int main()
{
	cv::Mat_<double> mat = (cv::Mat_<double>(3, 2) <<
							1.23, 1.23,
							1.23, 1.23,
							1.23, 1.23);
	writeTxt("mat.txt", mat);
//	readTxt("mat.txt", mat);
}
#endif

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

class ErrorFunction {
private:
    //1組のデータ
    const double x;
    const double y;
 
public:
    //データを1つずつ入力する際このコンストラクタが呼ばれる。
    ErrorFunction(double x, double y) : x(x), y(y) {}
 
    //e_iの定義。
    template <typename Type> bool operator()(const Type* const a, Type* residual) const {

        //e_i = y_i - a*x_i*x_i
        residual[0] = Type(y)  - a[0]*Type(x)*Type(x);
        return true;
    }
};

int main(int argc, char **argv)
{

    //最適化するパラメータ。初期値を入力しておく。
    double a = 0.0;
 
    //データの入力
    cv::Mat_<double> data;
    readTxt("data.txt", data);
 
    //データの入力
    Problem problem;
    for (int i = 0; i < data.rows; ++i) {

        //x_i,y_iを1つずつ入力する
        problem.AddResidualBlock(
            new AutoDiffCostFunction<ErrorFunction, 1, 1>
            (
                //ここでデータを入れている
                new ErrorFunction(data(i, 0), data(i, 1))
            ),
            NULL,    //Loss function
            &a
        );
        std::cout << data(i, 0) << ' ' << data(i, 1) << std::endl;
    }
 
    //ソルバの準備
    Solver::Options options;

    //CXSparseをインストールしていないときはDENSE_*にしないとエラーになる。
    options.linear_solver_type = ceres::DENSE_NORMAL_CHOLESKY;   
 
    //最適化
    Solver::Summary summary;
    Solve(options, &problem, &summary);
 
    std::cout << "Fit complete:   a: " << a << "\n";
    return 0;
}

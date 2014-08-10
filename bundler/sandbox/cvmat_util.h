// http://buaiso.blogspot.jp/2012/07/cvmat.html

#include <opencv2/opencv.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <fstream> 

template<typename Type>
bool writeTxt(const std::string filename, const cv::Mat_<Type> & mat)
{
    using std::string;
    string line;
    std::ofstream ofs(filename.c_str());
    if(!ofs)
    {
        std::cout << boost::format(" cannot open %s\n") % filename;
        return false;
    }
 
    for(int j=0; j<mat.rows; j++)
    {
        for(int i=0; i<mat.cols; i++)
        {
            ofs << mat(j, i);
            if(i < mat.cols - 1) ofs << " ";
            if(i == mat.cols - 1) ofs << "\n";
        }
    }

    return true;
}

template<typename Type>
bool readTxt(const std::string filename, cv::Mat_<Type> & mat)
{
    using std::string;
    string line;
    std::ifstream ifs(filename.c_str());
    if(!ifs)
    {
        std::cout << boost::format(" cannot open %s\n") % filename;
        return false;
    }
 
    mat = cv::Mat_<Type>();
 
    while( getline(ifs, line))
    {
        boost::trim(line);
        std::list<std::string> results;
        boost::split(results, line, boost::is_any_of(" \t"),
					 boost::token_compress_on);
 
        cv::Mat_<Type> row(1, results.size());
        std::list<std::string>::iterator iter = results.begin();
        std::list<std::string>::iterator tail = results.end();
        for(int i=0; iter != tail; ++iter, i++)
        {
            row(i) =  boost::lexical_cast<Type>(*iter);
        }
 
        mat.push_back(row);
    }

    return true;
}

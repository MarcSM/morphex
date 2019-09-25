/*
 ==============================================================================
 
 SMTUtils.h
 Created: 6 Jun 2019 10:12:08am
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include <vector>
#include <numeric>

/** Split a list of doubles in a string */
inline std::vector<double> splitDoubles(const std::string& list_of_doubles)
{
    std::string clean_list_of_doubles = list_of_doubles.substr(1, list_of_doubles.size() - 2); // Removing the "[]"
    clean_list_of_doubles.erase(std::remove(clean_list_of_doubles.begin(), clean_list_of_doubles.end(), ','), clean_list_of_doubles.end()); // Removing the ","
    
    std::istringstream iss(clean_list_of_doubles);
    
    return std::vector<double>{
        std::istream_iterator<double>(iss),
        std::istream_iterator<double>()
    };
}

/** Get a vector of doubles from an xml element */
inline std::vector<double> getVectorOfDoubles(XmlElement* parent, String child_name )
{
    std::vector<double> result_vector;
    
    forEachXmlChildElementWithTagName(*parent, child, child_name)
    {
        result_vector.push_back(child->getAllSubText().getDoubleValue());
    }
    
    return result_vector;
}

/** Split a list of floats in a string */
inline std::vector<float> splitFloats(const std::string& list_of_doubles)
{
    std::string clean_list_of_doubles = list_of_doubles.substr(1, list_of_doubles.size() - 2); // Removing the "[]"
    clean_list_of_doubles.erase(std::remove(clean_list_of_doubles.begin(), clean_list_of_doubles.end(), ','), clean_list_of_doubles.end()); // Removing the ","
    std::istringstream iss(clean_list_of_doubles);
    
    return std::vector<float>{
        std::istream_iterator<float>(iss),
        std::istream_iterator<float>()
    };
}

/** Get a matrix of floats from an xml element */
inline std::vector<std::vector<float>> getMatrixOfFloats(XmlElement* parent, String child_name )
{
    std::vector<std::vector<float>> result_matrix;
    
    // For each row of the matrix
    forEachXmlChildElementWithTagName(*parent, child, child_name)
    {
        auto aux = child->getAllSubText();
        result_matrix.push_back(splitFloats(child->getAllSubText().toStdString()));
    }
    
    return result_matrix;
}

/** Get a matrix of doubles from an xml element */
inline std::vector<std::vector<double>> getMatrixOfDoubles(XmlElement* parent, String child_name )
{
    std::vector<std::vector<double>> result_matrix;
    
    // For each row of the matrix
    forEachXmlChildElementWithTagName(*parent, child, child_name)
    {
        result_matrix.push_back(splitDoubles(child->getAllSubText().toStdString()));
    }
    
    return result_matrix;
}

/** Split a list of floats in a string */
inline std::pair<int, int> getMaxShape(std::vector<std::vector<float>> matrix_1,
                                       std::vector<std::vector<float>> matrix_2)
{
    for (int i = 0; i < matrix_1.size(); i++) {
        for (int j = 0; j < matrix_1[i].size(); j++)
            std::cout << matrix_1[i][j] << " ";
        std::cout << std::endl;
    }
    
    int max_len = (matrix_1.size() > matrix_2.size()) ? (int)matrix_1.size() : (int)matrix_2.size();
    int max_harmonics = (matrix_1[0].size() > matrix_2[0].size()) ? (int)matrix_1[0].size() : (int)matrix_2[0].size();
    
    return std::make_pair(max_len, max_harmonics);
}

/** Generate a vector within a range */
inline std::vector<int> getNonZeroIndices(std::vector<float> in_vector)
{
    std::vector<int> non_zero_indices;
    
    for (int i = 0; i < in_vector.size(); i++)
    {
        if (in_vector[i] > 0) non_zero_indices.push_back(i);
    }
    
    return non_zero_indices;
}

struct IncGenerator {
    int current_;
    IncGenerator (int start) : current_(start) {}
    int operator() () { return current_++; }
};

/** Generate a vector within a range */
inline std::vector<int> getRangeInt(int min, int max)
{
    int result_length = max-min;
    std::vector<int> result(result_length);
    std::iota(result.begin(), result.end(), min);
    
    return result;
}

struct RandomGenerator {
    double minValue;
    double maxValue;
    RandomGenerator(double min, double max) :
    minValue(min),
    maxValue(max) {
    }
    
    double operator()() {
        return (maxValue - minValue) * ( (double)std::rand() / (double)RAND_MAX ) + minValue;
    }
};

inline int CalculateModuloInt(int n, int M) {
    return ((n % M) + M) % M;
}

inline void PrintArrayDoubles(std::string element_name, double* element, int element_size)
{
    DBG("\n\n");
    DBG(element_name);
    
    for (int i = 0; i < element_size; i++)
    {
        double aux = element[i];
        std::cout << std::setprecision(10) << aux << ',';
    }
}

inline void PrintVectorFloats(std::string element_name, std::vector<float> element)
{
    DBG("\n\n");
    DBG(element_name);
    
    for (int i = 0; i < element.size(); i++)
    {
        float aux = element[i];
        std::cout << aux << ',';
    }
}

inline void PrintVectorDoubles(std::string element_name, std::vector<double> element)
{
    DBG("\n\n");
    DBG(element_name);
    
    for (int i = 0; i < element.size(); i++)
    {
        double aux = element[i];
        std::cout << aux << ',';
    }
}

inline void PrintArrayFloats(std::string element_name, float* element, int element_size)
{
    DBG("\n\n");
    DBG(element_name);
    
    for (int i = 0; i < element_size; i++)
    {
        float aux = element[i];
        std::cout << aux << ',';
    }
}

inline void PrintVectorComplexReal(std::string element_name, std::vector<std::complex<float>> element)
{
    DBG("\n\n");
    DBG(element_name);

    for (int i = 0; i < element.size(); i++)
    {
        float aux = element[i].real();
        std::cout << std::setprecision(10) << aux << ',';
    }
}

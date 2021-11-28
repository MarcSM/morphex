/* Copyright (C) 2020 Marc Sanchez Martinez
 *
 * https://github.com/MarcSM/morphex
 *
 * Morphex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Morphex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Morphex. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <numeric>
#include <vector>

// TEST REMOVE
#include <JuceHeader.h>

/** Check if an xml element has any child with a given name */
inline bool hasChild (std::unique_ptr<XmlElement> parent, String child_name)
{
    return parent->getChildByName (child_name) != nullptr;
}

/** Check if an xml element has any child with a given name */
inline bool hasChild (XmlElement* parent, String child_name)
{
    return parent->getChildByName (child_name) != nullptr;
}

/** Split a list of ints in a string */
inline std::vector<long long> splitInts (const std::string& list_of_ints)
{
    std::string clean_list_of_ints = list_of_ints.substr (1, list_of_ints.size() - 2); // Removing the "[]"
    std::replace_if (
        clean_list_of_ints.begin(), clean_list_of_ints.end(), [] (const char& c) { return c == ','; }, ' ');
    //        [] (const char& c) { return std::ispunct(c) ;},' ');
    std::stringstream      ss (clean_list_of_ints);
    std::vector<long long> result_int;

    long long num;
    while (ss >> num)
        result_int.push_back (num);

    return result_int;

    //    std::string clean_list_of_ints = list_of_ints.substr(1, list_of_ints.size() - 2); // Removing the "[]"
    //    clean_list_of_ints.erase(std::remove(clean_list_of_ints.begin(), clean_list_of_ints.end(), ','), clean_list_of_ints.end()); // Removing the ","
    //    std::istringstream iss(clean_list_of_ints);
    //
    //    return std::vector<int>{
    //        std::istream_iterator<int>(iss),
    //        std::istream_iterator<int>()
    //    };
}

/** Split a list of floats in a string */
inline std::vector<float> splitFloats (const std::string& list_of_floats, bool int_to_float = false)
{
    if (int_to_float)
    {
        std::string clean_list_of_ints = list_of_floats.substr (1, list_of_floats.size() - 2); // Removing the "[]"
        std::replace_if (
            clean_list_of_ints.begin(), clean_list_of_ints.end(), [] (const char& c) { return c == ','; }, ' ');
        //        [] (const char& c) { return std::ispunct(c) ;},' ');
        std::stringstream ss (clean_list_of_ints);
        ss.precision (20);
        std::vector<float> result_float;

        //        stringstream lineStream(line);

        // TODO - Check value precision (rounding when printing in scientific notation)
        //        std::vector<int> result_int;

        int num;
        while (ss >> num)
            result_float.push_back ((float) num);

        //        float num;
        //        while (ss >> num) result_float.push_back(num);

        //        copy(std::istream_iterator<float>(ss),
        //             std::istream_iterator<float>(),
        //             back_inserter(result_float));

        //        for (int i; ss >> i;) {
        //        int i;
        //        while(ss >> i) {
        //            result_float.push_back(i);
        ////            result_int.push_back(i);
        //            if (ss.peek() == ',' || ss.peek() == ' ')
        //                ss.ignore();
        //        }

        return result_float;
        //        std::string clean_list_of_floats = list_of_floats.substr(1, list_of_floats.size() - 2); // Removing the "[]"
        ////        clean_list_of_floats.erase(std::remove(clean_list_of_floats.begin(), clean_list_of_floats.end(), ','), clean_list_of_floats.end()); // Removing the ","
        //        std::istringstream iss(clean_list_of_floats);
        //
        //        return std::vector<float>{
        //            std::istream_iterator<float>((float)iss),
        //            std::istream_iterator<float>()
        //        };
    }
    else
    {
        // TODO - not remove commas, just skip them
        std::string clean_list_of_floats = list_of_floats.substr (1, list_of_floats.size() - 2);                                              // Removing the "[]"
        clean_list_of_floats.erase (std::remove (clean_list_of_floats.begin(), clean_list_of_floats.end(), ','), clean_list_of_floats.end()); // Removing the ","
        std::istringstream iss (clean_list_of_floats);

        return std::vector<float> {
            std::istream_iterator<float> (iss),
            std::istream_iterator<float>()
        };
    }
}

/** Split a list of doubles in a string */
inline std::vector<double> splitDoubles (const std::string& list_of_doubles)
{
    std::string clean_list_of_doubles = list_of_doubles.substr (1, list_of_doubles.size() - 2);                                               // Removing the "[]"
    clean_list_of_doubles.erase (std::remove (clean_list_of_doubles.begin(), clean_list_of_doubles.end(), ','), clean_list_of_doubles.end()); // Removing the ","

    std::istringstream iss (clean_list_of_doubles);

    return std::vector<double> {
        std::istream_iterator<double> (iss),
        std::istream_iterator<double>()
    };
}

/** Get a vector of ints from an xml element */
inline std::vector<long long> getVectorOfInts (XmlElement* parent, String child_name)
{
    return splitInts (parent->getChildByName (child_name)->getAllSubText().toStdString());
}

/** Get a vector of floats from an xml element */
inline std::vector<float> getVectorOfFloats (XmlElement* parent, String child_name)
{
    return splitFloats (parent->getChildByName (child_name)->getAllSubText().toStdString(), true);
}

/** Get a vector of doubles from an xml element */
inline std::vector<double> getVectorOfDoubles (XmlElement* parent, String child_name)
{
    std::vector<double> result_vector;

    forEachXmlChildElementWithTagName (*parent, child, child_name)
    {
        result_vector.push_back (child->getAllSubText().getDoubleValue());
    }

    return result_vector;
}

/** Get a matrix of ints from an xml element */
inline std::vector<std::vector<long long>> getMatrixOfInts (XmlElement* parent, String child_name)
{
    std::vector<std::vector<long long>> result_matrix;

    //    int i=1;
    // For each row of the matrix
    forEachXmlChildElementWithTagName (*parent, child, child_name)
    {
        //        if(i == 28){
        //            DBG("CHECK");
        //        }
        //        auto aux = child->getAllSubText();
        result_matrix.push_back (splitInts (child->getAllSubText().toStdString()));
        //        i++;
    }

    return result_matrix;
}

/** Get a matrix of floats from an xml element */
inline std::vector<std::vector<float>> getMatrixOfFloats (XmlElement* parent, String child_name)
{
    std::vector<std::vector<float>> result_matrix;

    //    int i=1;
    // For each row of the matrix
    forEachXmlChildElementWithTagName (*parent, child, child_name)
    {
        //        if(i == 28){
        //            DBG("CHECK");
        //        }
        //        auto aux = child->getAllSubText();
        result_matrix.push_back (splitFloats (child->getAllSubText().toStdString(), true));
        //        i++;
    }

    return result_matrix;
}

/** Get a matrix of doubles from an xml element */
inline std::vector<std::vector<double>> getMatrixOfDoubles (XmlElement* parent, String child_name)
{
    std::vector<std::vector<double>> result_matrix;

    // For each row of the matrix
    forEachXmlChildElementWithTagName (*parent, child, child_name)
    {
        result_matrix.push_back (splitDoubles (child->getAllSubText().toStdString()));
    }

    return result_matrix;
}

/** Split a list of floats in a string */
inline std::pair<int, int> getMaxShape (std::vector<std::vector<float>> matrix_1,
                                        std::vector<std::vector<float>> matrix_2)
{
    for (int i = 0; i < matrix_1.size(); i++)
    {
        for (int j = 0; j < matrix_1[i].size(); j++)
            std::cout << matrix_1[i][j] << " ";
        std::cout << std::endl;
    }

    int max_len       = (matrix_1.size() > matrix_2.size()) ? (int) matrix_1.size() : (int) matrix_2.size();
    int max_harmonics = (matrix_1[0].size() > matrix_2[0].size()) ? (int) matrix_1[0].size() : (int) matrix_2[0].size();

    return std::make_pair (max_len, max_harmonics);
}

/** Generate a vector within a range */
inline std::vector<int> getNonZeroIndices (std::vector<float> in_vector)
{
    std::vector<int> non_zero_indices;

    for (int i = 0; i < in_vector.size(); i++)
    {
        if (in_vector[i] > 0)
            non_zero_indices.push_back (i);
    }

    return non_zero_indices;
}

struct IncGenerator
{
    int current_;
    IncGenerator (int start) :
        current_ (start) {}
    int operator()() { return current_++; }
};

struct RandomGenerator
{
    double minValue;
    double maxValue;
    RandomGenerator (double min, double max) :
        minValue (min),
        maxValue (max)
    {
    }

    double operator()()
    {
        return (maxValue - minValue) * ((double) std::rand() / (double) RAND_MAX) + minValue;
    }
};

inline int CalculateModuloInt (int n, int M)
{
    return ((n % M) + M) % M;
}

inline void PrintArrayDoubles (std::string element_name, double* element, int element_size)
{
    DBG ("\n\n");
    DBG (element_name);

    for (int i = 0; i < element_size; i++)
    {
        double aux = element[i];
        std::cout << std::setprecision (10) << aux << ',';
    }
}

inline void PrintVectorFloats (std::string element_name, std::vector<float> element)
{
    DBG ("\n\n");
    DBG (element_name);

    for (int i = 0; i < element.size(); i++)
    {
        float aux = element[i];
        std::cout << aux << ',';
    }
}

inline void PrintVectorDoubles (std::string element_name, std::vector<double> element)
{
    DBG ("\n\n");
    DBG (element_name);

    for (int i = 0; i < element.size(); i++)
    {
        double aux = element[i];
        std::cout << aux << ',';
    }
}

inline void PrintArrayFloats (std::string element_name, float* element, int element_size)
{
    DBG ("\n\n");
    DBG (element_name);

    for (int i = 0; i < element_size; i++)
    {
        float aux = element[i];
        std::cout << aux << ',';
    }
}

inline void PrintVectorComplexReal (std::string element_name, std::vector<std::complex<float>> element)
{
    DBG ("\n\n");
    DBG (element_name);

    for (int i = 0; i < element.size(); i++)
    {
        float aux = element[i].real();
        std::cout << std::setprecision (10) << aux << ',';
    }
}

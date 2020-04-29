/*
  ==============================================================================

    Codec.h
    Created: 22 Apr 2020 6:06:39pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

namespace Core
{
    const static float DEFAULT_HZ = 0.0;
    const static float DEFAULT_DB = -180.0;
    const static int XML_DECIMAL_PLACES = 5;
}

namespace Core::Codec
{
//    const float DEFAULT_FREQ = 0.0;
//    const float DEFAULT_MAG = -180.0;
//    const float DEFAULT_STOC = -180.0;
//    const float XML_DECIMAL_PLACES = 5;
    
    // List Decoding (does not change size of arrays)
    void inline decodeVector(std::vector<float> &given_list, int abs_values = false, int decimal_places = XML_DECIMAL_PLACES)
    {
        for (int i = 0; i < given_list.size(); i++)
        {
            // Get rid of the "-" on mags and stocs
            if (abs_values) given_list[i] = -given_list[i];
            
            // Get rid of the dots "."
            given_list[i] /= std::pow(10, decimal_places);
        }
    }
    
    // Matrix Decoding
    void inline decodeMatrix(std::vector<std::vector<float>> &given_matrix, bool abs_values = false, bool diff_decoding = true, int decimal_places = XML_DECIMAL_PLACES)
    {
        for (int i = 0; i < given_matrix.size(); i++)
        {
            decodeVector(given_matrix[i], abs_values, decimal_places);
            
            if (diff_decoding && i > 0)
            {
                for (int j = 0; j < given_matrix[i].size(); j++)
                {
                    if (j < given_matrix[i-1].size())
                    {
                        given_matrix[i][j] = given_matrix[i-1][j] + given_matrix[i][j];
                    }
                }
            }
        }
    }
    
    // Matrix Padding
    void inline padMatrix(std::vector<std::vector<float>> X, int max_x, int max_y, float pad_value = 0.0)
    {
        for (int i = 0; i < max_y; i++)
        {
            // TODO
//            if (i < X.size())
//            {
//                X[i] = X[i] + ( [pad_value] * ( max_x - X[i].size() ) )
//            }
//            else
//            {
//                X.push_back( [pad_value] * max_x )
//            }
        }
    }
};

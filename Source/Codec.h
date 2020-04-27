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
    void decodeList(std::vector<float> given_list, int abs_values = false, int decimal_places = XML_DECIMAL_PLACES)
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
    void decodeMatrix(std::vector<std::vector<float>> given_matrix, bool abs_values = false, bool diff_decoding = true, int decimal_places = XML_DECIMAL_PLACES)
    {
        for (int i = 0; i < sizeof(given_matrix); i++)
        {
            decodeList(given_matrix[i], decimal_places, abs_values);
            
            if (diff_decoding and i > 0)
            {
                for (int j = 0; j < sizeof(given_matrix[i]); j++)
                {
                    if (j < sizeof(given_matrix[i-1]))
                    {
                        given_matrix[i][j] = given_matrix[i-1][j] + given_matrix[i][j];
                    }
                }
            }
        }
    }
    
    // Matrix Padding
    void padMatrix(std::vector<std::vector<float>> X, int max_x, int max_y, float pad_value = 0.0)
    {
        for (int i = 0; i < max_y; i++)
        {
            // TODO
//            if (i < sizeof(X))
//            {
//                X[i] = X[i] + ( [pad_value] * ( max_x - sizeof(X[i]) ) )
//            }
//            else
//            {
//                X.push_back( [pad_value] * max_x )
//            }
        }
    }
};

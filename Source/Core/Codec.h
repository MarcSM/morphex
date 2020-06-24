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

namespace Core
{
    const static float DEFAULT_HZ = 0.0;
    const static float DEFAULT_DB = -200.0;
    const static float DEFAULT_NULL = -1;
    const static int XML_DECIMAL_PLACES = 3;
}

namespace Core::Codec
{
    // List Decoding (does not change size of arrays)
    template<typename T>
    void inline decodeVector (std::vector<T> &given_list, int abs_values = false, int decimal_places = XML_DECIMAL_PLACES)
    {
        for (int i = 0; i < given_list.size(); i++)
        {
            // Recover the "-" on mags and stocs
            if (abs_values) given_list[i] = -given_list[i];
            
            // Recover the decmimal values ","
            given_list[i] /= std::pow (10, decimal_places);
        }
    }

    // Matrix Diff Decoding
    template <typename T>
    void inline decodeMatrixDiff (std::vector<std::vector<T>> &given_matrix)
    {
        for (int i = 1; i < given_matrix.size(); i++)
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
    
    // Matrix Decoding
    void inline decodeMatrix (std::vector<std::vector<float>> &given_matrix, bool abs_values = false, bool diff_decoding = true, int decimal_places = XML_DECIMAL_PLACES)
    {
        if (diff_decoding) decodeMatrixDiff (given_matrix);
        
        for (int i = 0; i < given_matrix.size(); i++)
        {
            decodeVector (given_matrix[i], abs_values, decimal_places);
        }
    }
};

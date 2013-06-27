/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @brief     This file contains definition of CComputeCheckSum class
 * @author    Anish Kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains definition of CComputeCheckSum class
 */

#pragma once

class CComputeCheckSum
{
protected:
	/**
	 * @brief compute the check for a buffer pass as parameter
	 * @param[in] pcBuff pointer to buffer
	 * @param[in] dwSize number of bytes for which checksum to be computed
	 * @param[out] pucCheckSum pointer to computed checksum byte value
	 * @return TRUE on success
	 *
     * This method computes the checksum for a give bytes
     * of data passed as pointer pcBuff.
     */
    BOOL computeCheckSum(UCHAR* pcBuff,DWORD dwSize, UCHAR* pucCheckSum);

	/**
	 * @brief Get the computed checksum of file
	 * @param[in] omStrConfigFileName reference to File name
	 * @param[out] pucCheckSum Returns computed check sum
	 * @param[out] pucCheckSumInFile Returns the chechsum in file
	 * @return TRUE on success and FALSE for failure.
	 *
     * This method computes the checksum for a given bytes
     * in a file. The file is opened and checksum is computed
     * for all bytes except the last byte.
	 */
    BOOL getCheckSum(CString& omStrConfigFileName, UCHAR* pucCheckSum,
                      UCHAR* pucCheckSumInFile);

    /**
	 * @brief Sets the checksum at last byte location after computing it.
	 * @param[in] omStrConfigFileName reference to File name
	 * @param[out] pucCheckSum Returns computed check sum
	 * @return TRUE for success and FALSE for failure
	 *
	 * This method computes the checksum for a given bytes
	 * in a file. The file is opened and checksum is computed
	 * for all bytes except the last byte. The same checksum is
	 * overwritten at the last byte in file.
	 */
    BOOL setCheckSum(CString& omStrConfigFileName,UCHAR* pucCheckSum);

	/**
	 * @param[in] omStrConfigFileName reference to File name
	 * @param[out] pucCheckSum Returns computed check sum
	 * @return TRUE for success and FALSE for failure
	 *
	 * This method computes the checksum for a given bytes
	 * in a file. The file is opened and checksum is computed
	 * for all bytes except the last byte. The same checksum is
	 * overwritten at the last byte in file.
	 *
	 * For COM Support
	 */
    BOOL setCheckSumViaCom(CString& omStrConfigFileName,
                          UCHAR* pucCheckSum, CString& strError);
};

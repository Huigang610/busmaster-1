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
 * @brief     Definition of CRefTimeKeeper class
 * @author    Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CRefTimeKeeper class
 */

#pragma once

class CRefTimeKeeper
{
public:
	static void setTimeParameters(SYSTEMTIME& currentSystemTime, UINT64 connectTime);
	static void getTimeParameters(UINT64& referenceSystemTime_, UINT64& absoluteBaseTime_);

protected:
    static UINT64 referenceSystemTime;
    static UINT64 absoluteBaseTime;
};

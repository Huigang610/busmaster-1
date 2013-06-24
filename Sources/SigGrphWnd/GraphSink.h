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
 * @brief     Implementation file for CGraphBottomView class
 * @author    Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CGraphBottomView class
 */

#pragma once

/* Project includes */
#include "GraphChildFrame.h"

class CGraphSink : public CCmdTarget
{
    DECLARE_DISPATCH_MAP()
    DECLARE_DYNAMIC(CGraphSink)
    DECLARE_INTERFACE_MAP()

public:
    CGraphSink();

    virtual void OnFinalRelease();
    void OnCursorPosition(double x, double y, short shCursorID);

    CGraphChildFrame* m_pParentWnd;
};



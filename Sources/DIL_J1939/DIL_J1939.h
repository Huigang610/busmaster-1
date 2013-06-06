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
 * @file   J1939Translator.h
 * @brief  J1939 Translator application class
 * @author Pradeep Kadoor
 * @copyricht Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *
 * J1939 Translator application class
 */

#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

class CJ1939TranslatorApp : public CWinApp
{
public:
    CJ1939TranslatorApp();

    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

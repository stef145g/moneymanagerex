/*******************************************************
Copyright (C) 2017 Stefano Giorgio

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************/

#pragma once
//----------------------------------------------------------------------------
#include <cppunit/extensions/HelperMacros.h>
//----------------------------------------------------------------------------
#include "option.h"

class TestFrameBase;

class Test_JSON : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Test_JSON);
    CPPUNIT_TEST(TestJson_db_table_accountlist);
    CPPUNIT_TEST(TestJson_PayeeListToJson);
    CPPUNIT_TEST(TestJson_CategoryListToJson);
    CPPUNIT_TEST_SUITE_END();

public:
    Test_JSON();
    virtual ~Test_JSON();

    void setUp();
    void tearDown();

private:
    wxString m_test_db_filename;
    wxSQLite3Database m_test_db;
    int m_this_instance;

private:
    // Test cases
    void TestJson_db_table_accountlist();
    void TestJson_PayeeListToJson();
    void TestJson_CategoryListToJson();
};
//----------------------------------------------------------------------------

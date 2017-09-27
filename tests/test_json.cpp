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
Foundation, Inc., 59 Temple Placeuite 330, Boston, MA  02111-1307  USA
 ********************************************************/

#include "defs.h"
#include <cppunit/config/SourcePrefix.h>
#include "cpu_timer.h"
#include "framebase_tests.h"
//----------------------------------------------------------------------------
#include "test_json.h"
#include <wx/frame.h>
#include "reports/mmDateRange.h"

/*****************************************************************************
Turn test ON or OFF in file: defined_test_selection.h
*****************************************************************************/
#include "defined_test_selection.h"

#include "db\DB_Table_Accountlist_V1.h"

#ifdef MMEX_TESTS_JSON
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(Test_JSON);
#endif

static int s_instance_count = 0;
//----------------------------------------------------------------------------
Test_JSON::Test_JSON()
{
    s_instance_count++;
    m_this_instance = s_instance_count;
    m_test_db_filename = "test_db_json.mmb";
    if ((m_this_instance == 1) && wxFileExists(m_test_db_filename))
    {
        wxRemoveFile(m_test_db_filename);
    }
}

Test_JSON::~Test_JSON()
{
    s_instance_count--;
    if (s_instance_count < 1)
    {
        wxRemoveFile(m_test_db_filename);
    }
}

void Test_JSON::setUp()
{
    CpuTimer time("Startup");
    m_test_db.Open(m_test_db_filename);
}

void Test_JSON::tearDown()
{
    m_test_db.Close();
}

//--------------------------------------------------------------------------

void Test_JSON::TestJson_db_table_accountlist()
{
    DB_Table_ACCOUNTLIST_V1 account_table;
    DB_Table_ACCOUNTLIST_V1::Data account_record;

    account_table.ensure(&m_test_db);

    wxString ar = account_record.to_json();

}

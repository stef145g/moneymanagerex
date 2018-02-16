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
#include "db_init_model.h"
#include "model/Model_Payee.h"
#include "model/Model_Category.h"

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

void Test_JSON::TestJson_PayeeListToJson()
{
    DB_Model* database = new DB_Model();
    database->Init_Model_Tables(&m_test_db);
    database->Init_BaseCurrency();

    database->Add_Payee("Coles", "Food", "Groceries");
    database->Add_Payee("Woolworths", "Food", "Groceries");
    database->Add_Payee("Aldi", "Food", "Groceries");

    int i = 0;
    json::Object jsonPayeeList;
    std::wstringstream jsonPayeeStream;
    wxString output_message, def_category_name, def_subcategory_name;

    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);
    json_writer.StartObject();
    json_writer.Key("Payees");

    json_writer.StartArray();
    for (const auto &Payee : Model_Payee::instance().all(Model_Payee::COL_PAYEENAME))
    {
        const Model_Category::Data* DefCategory = Model_Category::instance().get(Payee.CATEGID);
        if (DefCategory != nullptr)
            def_category_name = DefCategory->CATEGNAME;
        else
            def_category_name = "None";

        const Model_Subcategory::Data* DefSubCategory = Model_Subcategory::instance().get(Payee.SUBCATEGID);
        if (DefSubCategory != nullptr)
            def_subcategory_name = DefSubCategory->SUBCATEGNAME;
        else
            def_subcategory_name = "None";

        jsonPayeeList[L"Payees"][i][L"PayeeName"] = json::String(Payee.PAYEENAME.ToStdWstring());
        jsonPayeeList[L"Payees"][i][L"DefCateg"] = json::String(def_category_name.ToStdWstring());
        jsonPayeeList[L"Payees"][i][L"DefSubCateg"] = json::String(def_subcategory_name.ToStdWstring());

        i++;

        json_writer.StartObject();
        json_writer.Key("PayeeName");
        json_writer.String(Payee.PAYEENAME);
        json_writer.Key("DefCateg");
        json_writer.String(def_category_name);
        json_writer.Key("DefSubCateg");
        json_writer.String(def_subcategory_name);
        json_writer.EndObject();
    }

    json::Writer::Write(jsonPayeeList, jsonPayeeStream);
    wxString PayeesList = jsonPayeeStream.str();

    json_writer.EndArray();
    json_writer.EndObject();
    wxString payee_list = json_buffer.GetString();

    wxLogDebug("======= TestJson_PayeeListToJson =======");
    wxLogDebug("Payee_list using CAJUN:JSON:\n%s", PayeesList);
    wxLogDebug("Payee_list using rapidjson:\n%s", payee_list);

    delete database;
}

void Test_JSON::TestJson_CategoryListToJson()
{
    DB_Model* database = new DB_Model();
    database->Init_Model_Tables(&m_test_db);
    database->Init_BaseCurrency();

    int i = 0;
    json::Object jsonCategoryList;
    std::wstringstream jsonCategoryStream;
    //wxString SubCategoryName;

    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();
    json_writer.Key("Categories");

    json_writer.StartArray();

    const auto &categories = Model_Category::instance().all();
    for (const Model_Category::Data& category : categories)
    {
        bool FirstCategoryRun = true;
        bool SubCategoryFound = false;

        jsonCategoryList[L"Categories"][i][L"CategoryName"] = json::String(category.CATEGNAME.ToStdWstring());

        json_writer.StartObject();
        json_writer.Key("CategoryName");
        json_writer.String(category.CATEGNAME);

        for (const auto &sub_category : Model_Category::sub_category(category))
        {
            SubCategoryFound = true;
            if (FirstCategoryRun == true)
            {
                jsonCategoryList[L"Categories"][i][L"SubCategoryName"] = json::String(sub_category.SUBCATEGNAME.ToStdWstring());
                i++;

                json_writer.Key("SubCategoryName");
                json_writer.String(sub_category.SUBCATEGNAME);
                json_writer.EndObject();

                FirstCategoryRun = false;
            }
            else
            {
                jsonCategoryList[L"Categories"][i][L"CategoryName"] = json::String(category.CATEGNAME.ToStdWstring());
                jsonCategoryList[L"Categories"][i][L"SubCategoryName"] = json::String(sub_category.SUBCATEGNAME.ToStdWstring());
                i++;

                json_writer.StartObject();
                json_writer.Key("CategoryName");
                json_writer.String(category.CATEGNAME);

                json_writer.Key("SubCategoryName");
                json_writer.String(sub_category.SUBCATEGNAME);
                json_writer.EndObject();

                FirstCategoryRun = false;
            }
        }

        if (SubCategoryFound == false)
        {
            jsonCategoryList[L"Categories"][i][L"SubCategoryName"] = json::String(L"None");

            json_writer.Key("SubCategoryName");
            json_writer.String("None");
            json_writer.EndObject();
        }
        else
        {
            i--;
        }

        i++;
    }

    json_writer.EndArray();
    json_writer.EndObject();
    wxString category_list = json_buffer.GetString();

    json::Writer::Write(jsonCategoryList, jsonCategoryStream);
    wxString CategoryList = jsonCategoryStream.str();


    wxLogDebug("======= TestJson_CategoryListToJson =======");
    wxLogDebug("CategoryList using CAJUN:JSON:\n%s", CategoryList);

    wxLogDebug("CategoryList using rapidjson:\n%s", category_list);

    delete database;
}

void Test_JSON::TestJson_ModelChecking_FullDataToJson()
{
    DB_Model* database = new DB_Model();
    database->Init_Model_Tables(&m_test_db);
    database->Init_BaseCurrency();

    int account_id = database->Add_Bank_Account("Savings", 1200.00);
    int trans_id = database->Add_Trans_Withdrawal("Savings", wxDateTime::Today(), "Coles", 100.20);
    database->Add_Trans_Split(trans_id, 60.20, "Food", "Groceries");
    database->Add_Trans_Split(trans_id, 40.00, "Food", "Groceries");

    auto trans = Model_Checking::instance().get(trans_id);
    const auto splits = Model_Splittransaction::instance().get_all();
    Model_Checking::Full_Data full_trans(account_id, *trans, splits);
    wxString json_transaction = full_trans.to_json();
    

    int trans_nsid = database->Add_Trans_Deposit("Savings", wxDateTime::Today(), "Aldi", 75.25, "Income", "Salary", "Testing no splits");
    Model_Checking::Full_Data ft_noSplit(account_id, *Model_Checking::instance().get(trans_nsid));
    auto json_trans = ft_noSplit.to_json();

    delete database;
}

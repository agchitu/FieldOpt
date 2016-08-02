#include <gtest/gtest.h>
#include "ERTWrapper/eclsummaryreader.h"
#include "ERTWrapper/ertwrapper_exceptions.h"
#include "Utilities/tests/test_resource_example_file_paths.h"

using namespace ERTWrapper::ECLSummary;

namespace {

    class ECLSummaryReaderTest : public ::testing::Test {
    protected:
        ECLSummaryReaderTest() {
            ecl_summary_reader_ = new ECLSummaryReader(file_name_);
        }

        virtual ~ECLSummaryReaderTest() {
            delete ecl_summary_reader_;
        }

        virtual void SetUp() { }

        virtual void TearDown() { }

        ECLSummaryReader *ecl_summary_reader_;
        QString file_name_ = TestResources::ExampleFilePaths::ecl_base_horzwell;
    };

    TEST_F(ECLSummaryReaderTest, ReportSteps) {
        EXPECT_EQ(0, ecl_summary_reader_->GetFirstReportStep());
        EXPECT_EQ(22, ecl_summary_reader_->GetLastReportStep());
        EXPECT_TRUE(ecl_summary_reader_->HasReportStep(10));
        EXPECT_FALSE(ecl_summary_reader_->HasReportStep(-1));
        EXPECT_FALSE(ecl_summary_reader_->HasReportStep(32));
    }

    TEST_F(ECLSummaryReaderTest, MiscVar) {
        EXPECT_NO_THROW(ecl_summary_reader_->GetMiscVar("TIME", 1));
        EXPECT_THROW(ecl_summary_reader_->GetMiscVar("NOVAR", 1), ERTWrapper::SummaryVariableDoesNotExistException);
        EXPECT_THROW(ecl_summary_reader_->GetMiscVar("TIME", 23), ERTWrapper::SummaryTimeStepDoesNotExistException);
        EXPECT_FLOAT_EQ(200, ecl_summary_reader_->GetMiscVar("TIME", 22));
        EXPECT_FLOAT_EQ(0, ecl_summary_reader_->GetMiscVar("TIME", 0));
    }

    TEST_F(ECLSummaryReaderTest, FieldVar) {
        EXPECT_NO_THROW(ecl_summary_reader_->GetFieldVar("FOPT", 0));
        EXPECT_THROW(ecl_summary_reader_->GetFieldVar("NOVAR", 1), std::runtime_error);
        EXPECT_THROW(ecl_summary_reader_->GetFieldVar("FOPT", 32), ERTWrapper::SummaryTimeStepDoesNotExistException);
        EXPECT_FLOAT_EQ(0, ecl_summary_reader_->GetFieldVar("FOPT", 0));
        EXPECT_FLOAT_EQ(187866.44, ecl_summary_reader_->GetFieldVar("FOPT", 22));

        for (int i = 0; i < ecl_summary_reader_->GetLastReportStep(); ++i) {
            std::cout << ecl_summary_reader_->GetFieldVar("FOPT", i) << std::endl;
        }
        for (int i = 0; i < ecl_summary_reader_->GetLastReportStep(); ++i) {
            std::cout << ecl_summary_reader_->GetMiscVar("TIME", i) << std::endl;
        }
    }

    TEST_F(ECLSummaryReaderTest, WellVar) {
        EXPECT_NO_THROW(ecl_summary_reader_->GetWellVar("PROD", "WOPR", 1));
        EXPECT_THROW(ecl_summary_reader_->GetWellVar("NOWELL", "WOPR", 1), ERTWrapper::SummaryVariableDoesNotExistException);
        EXPECT_THROW(ecl_summary_reader_->GetWellVar("PROD", "NOVAR", 1), ERTWrapper::SummaryVariableDoesNotExistException);
        EXPECT_THROW(ecl_summary_reader_->GetWellVar("PROD", "WOPR", 32), ERTWrapper::SummaryTimeStepDoesNotExistException);
        EXPECT_FLOAT_EQ(0, ecl_summary_reader_->GetWellVar("PROD", "WOPR", 0));
        EXPECT_FLOAT_EQ(628.987, ecl_summary_reader_->GetWellVar("PROD", "WOPR", 22));
    }

    TEST_F(ECLSummaryReaderTest, TimeVector) {
        EXPECT_EQ(21, ecl_summary_reader_->time().size());
        EXPECT_EQ(0, ecl_summary_reader_->time()[0]);
        EXPECT_EQ(200, ecl_summary_reader_->time().back());
    }

    TEST_F(ECLSummaryReaderTest, WellCumulativeVectors) {
        EXPECT_EQ(21, ecl_summary_reader_->wopt("PROD").size());
        EXPECT_EQ(21, ecl_summary_reader_->wwpt("PROD").size());
        EXPECT_EQ(21, ecl_summary_reader_->wgpt("PROD").size());
        EXPECT_FLOAT_EQ(0.0, ecl_summary_reader_->wopt("PROD").front());
        EXPECT_FLOAT_EQ(0.0, ecl_summary_reader_->wwpt("PROD").front());
        EXPECT_FLOAT_EQ(0.0, ecl_summary_reader_->wgpt("PROD").front());
        EXPECT_FLOAT_EQ(187866.44, ecl_summary_reader_->wopt("PROD").back());
    }

    TEST_F(ECLSummaryReaderTest, FieldCumulativeVectors) {
        auto fopt = ecl_summary_reader_->fopt();
        auto fwpt = ecl_summary_reader_->fwpt();
        auto fgpt = ecl_summary_reader_->fgpt();
        auto fwit = ecl_summary_reader_->fwit();
        auto fgit = ecl_summary_reader_->fgit();
        EXPECT_EQ(21, fopt.size());
        EXPECT_EQ(21, fwpt.size());
        EXPECT_EQ(21, fgpt.size());
        EXPECT_EQ(21, fwit.size());
        EXPECT_EQ(21, fgit.size());

        EXPECT_FLOAT_EQ(0.0, fopt.front());
        EXPECT_FLOAT_EQ(187866.44, fopt.back());
        EXPECT_FLOAT_EQ(0.0, fwpt.front());
        EXPECT_FLOAT_EQ(1116.8876, fwpt.back());
    }

    TEST_F(ECLSummaryReaderTest, WellRateVectors) {
        auto wopr = ecl_summary_reader_->wopr("PROD");
        EXPECT_EQ(21, wopr.size());
        EXPECT_FLOAT_EQ(0.0, wopr.front());
        EXPECT_FLOAT_EQ(628.9869, wopr.back());
    }

    TEST_F(ECLSummaryReaderTest, KeysAndWells) {
        EXPECT_EQ(1, ecl_summary_reader_->wells().size());
        EXPECT_STREQ("PROD", ecl_summary_reader_->wells().first().toLatin1());
        EXPECT_EQ(11, ecl_summary_reader_->keys().size());
        EXPECT_EQ(4, ecl_summary_reader_->field_keys().size());
        EXPECT_EQ(6, ecl_summary_reader_->well_keys().size());
    }

}

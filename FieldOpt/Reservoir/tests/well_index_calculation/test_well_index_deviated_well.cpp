//
// Created by bellout on 7/30/16.
//

#include <gtest/gtest.h>
#include "Reservoir/grid/grid.h"
#include "Reservoir/grid/eclgrid.h"
#include "Reservoir/grid/grid_exceptions.h"
#include "Utilities/file_handling/filehandling.h"
#include "Reservoir/well_index_calculation/wellindexcalculator.h"

#include "Reservoir/tests/test_resource_wic_welldir.h"
#include "Reservoir/tests/test_resource_wic_diff_functions.h"

#include <QList>
#include <QVector>
#include <QProcess>
#include <QTextStream>
#include <fstream>
#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Reservoir::Grid;
using namespace Reservoir::WellIndexCalculation;
using namespace TestResources::WIBenchmark;

namespace {

    class DeviatedWellIndexTest : public ::testing::Test {

    protected:
        DeviatedWellIndexTest() {
            grid_ = new ECLGrid(file_path_);
            well_dir_ = new WellDir();
        }
        virtual ~DeviatedWellIndexTest() {
            delete grid_;
        }
        Grid *grid_;
        WellDir *well_dir_;
        QString file_path_ = "../examples/ADGPRS/5spot/ECL_5SPOT.EGRID";
        bool debug_ = false;
    };

    TEST_F(DeviatedWellIndexTest, test) {

    }

    TEST_F(DeviatedWellIndexTest, compareCOMPDAT) {

        // FIND LIST OF WELL FOLDERS CONTAINING PCG & RMS COMPDATS
        auto file_list_ = well_dir_->GetWellDir();
        auto dir_names_ = file_list_[0]; // list of well dirs (names only) => dir name only: tw04_04
        auto dir_list_  = file_list_[1]; // list of well dirs (absolute path) => fullpath: ../tw04_04/
        auto rms_files  = file_list_[2]; // fullpath: ../tw04_04/EVENTS_tw04_04_RMS.DATA
        auto pcg_files  = file_list_[3]; // fullpath: ../tw04_04/EVENTS_tw04_04_PCG.DATA

        if (debug_) {
            std::cout << "\033[1;31m<DEBUG:START->\033[0m" << std::endl;
            for (int ii = 0; ii < dir_names_.length(); ++ii) {
                std::cout << ii << ":" << dir_names_[ii].toStdString() << std::endl; // list of well dirs (names only)
                std::cout << ii << ":" << dir_list_[ii].toStdString() << std::endl; // list of well dirs (absolute path)
            }
            std::cout << "\033[1;31m<DEBUG:END--->\033[0m" << std::endl;
        }

        // WELL INDEX DATA OBJECTS
        WIData WIDataRMS, WIDataPCG;

        // LOOP THROUGH LIST OF WELL FOLDERS: FOR WELL FOLDER ii,
        // READ PCG & RMS COMPDAT DATA
        int num_files = (debug_) ? 1 : rms_files.length();

        for( int ii=0; ii < num_files; ++ii ){

            // READ COMPDAT FILES
            WIDataRMS.ReadData(rms_files[ii]);
            WIDataPCG.ReadData(pcg_files[ii]);

            QStringList diff_files = {
                dir_list_[ii] + "/DIFF_" + dir_names_[ii] +  "_RMS.IJK",
                dir_list_[ii] + "/DIFF_" + dir_names_[ii] +  "_PCG.IJK",
                dir_list_[ii] + "/DIFF_" + dir_names_[ii] +  "_RMS.WCF",
                dir_list_[ii] + "/DIFF_" + dir_names_[ii] +  "_PCG.WCF"
            };

            // PRINT IJK, WCF DATA TO INDIVIDUAL FILES FOR DIFF TREATMENT
            WIDataRMS.PrintIJKData(diff_files[0]);
            WIDataPCG.PrintIJKData(diff_files[1]);

            WIDataRMS.PrintWCFData(diff_files[2]);
            WIDataPCG.PrintWCFData(diff_files[3]);


            // MESSAGE OUTPUT
            std::cout << "\n\n\033[1;36m" << std::setfill('=') << std::setw(80) << "=" << "\033[0m" << std::endl;
            std::cout << "\033[1;36mChecking IJK and WCF data for well: "
                      << WIDataRMS.dir_name.toStdString() << "\033[0m" << std::endl;
            std::cout << "\033[1;36m" << std::setfill('=') << std::setw(80) << "=" << "\033[0m" << std::endl;

            if (debug_){
                int nRMS = (WIDataRMS.IJK.rows() > 5) ? 5 : WIDataRMS.IJK.rows();
                int nPCG = (WIDataPCG.IJK.rows() > 5) ? 5 : WIDataPCG.IJK.rows();
                // std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
                std::cout << "\033[1;31m<DEBUG:START->\033[0m" << std::endl;

                // RMS-PCG: IJK
                std::cout << "RMS-PCG IJK DATA (well = " << dir_names_[ii].toStdString() << ")" << std::endl;
                // RMS: IJK
                std::cout << "WIDataRMS.IJK (size: " << WIDataRMS.IJK.size() << "): "
                          << std::endl << WIDataRMS.IJK.block(0,0,nRMS,4)
                          << std::endl << "..." << std::endl;
                // PCG: IJK
                std::cout << "WIDataPCG.IJK (size: " << WIDataPCG.IJK.size() << "): "
                          << std::endl << WIDataPCG.IJK.block(0,0,nPCG,4)
                          << std::endl << "..." << std::endl;

                // RMS-PCG: WCF
                std::cout << "RMS-PCG WCF DATA (well = " << dir_names_[ii].toStdString() << ")" << std::endl;
                // RMS: WCF
                std::cout << "WIDataRMS.WCF: (size: " << WIDataRMS.WCF.size() << "): "
                          << std::endl << WIDataRMS.WCF.block(0,0,nRMS,1)
                          << std::endl << "..." << std::endl;
                // PCG: WCF
                std::cout << "WIDataPCG.WCF: (size: " << WIDataPCG.WCF.size() << "): "
                          << std::endl << WIDataPCG.WCF.block(0,0,nPCG,1)
                          << std::endl << "..." << std::endl;

                std::cout << "\033[1;31m<DEBUG:END--->\033[0m" << std::endl;
                // std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
            }

            if(DiffVectorLength(WIDataRMS,WIDataPCG)) {

                // IF VECTOR LENGTHS ARE EQUAL => COMPARE DIRECTLY
                std::cout << "\033[1;36m" << WIDataRMS.dir_name.toStdString() <<
                ": >>> Vector lengths are equal. Making comparison.\033[0m" <<
                std::setfill(' ') << std::endl;

                CompareIJK(WIDataRMS, WIDataPCG);
                CompareWCF(WIDataRMS, WIDataPCG);

            }else{

                // IF VECTOR LENGTHS ARE UNEQUAL => MAKE EQUAL, THEN COMPARE DIRECTLY
                std::cout << "\033[1;36m" << WIDataRMS.dir_name.toStdString() <<
                ": >>> Vector lengths are unequal. Making them equal.\033[0m" << std::endl;

                // diff options
                // -q report only when files differ
                // -c output NUM (default 3) lines of copied context
                // -Z  ignore white space at line end
                // -b ignore changes in the amount of white space
                // -w --ignore-all-space
                // diff output
                // %<     lines from FILE1
                // %>     lines from FILE2
                // %=     lines common to FILE1 and FILE2

                // DIFF TREATMENT: IJK COMPARISON
                QProcess* diff_process = new QProcess;
                diff_process->start("diff", QStringList() << diff_files[0] << diff_files[1]);
                diff_process->waitForFinished();
                QString IJK_diff = diff_process->readAllStandardOutput();

                // DIFF TREATMENT: IJK COMPARISON
                diff_process->start("diff", QStringList() << diff_files[2] << diff_files[3] << "-q ");
                diff_process->waitForFinished();
                QString WCF_diff = diff_process->readAllStandardOutput();

                std::cout << "\n\nIJK_diff: " << WCF_diff.toStdString() << std::endl;
                std::cout << "\n\nWCF_diff: " << WCF_diff.toStdString() << std::endl;

            }



//        if(TestResources::WIBenchmark::DiffVectorLength(WIDataRMS,WIDataPCG)){
//            TestResources::WIBenchmark::CompareIJK(WIDataRMS,WIDataPCG);
//        }
//
//        TestResources::WIBenchmark::CompareIJK(WIDataRMS,WIDataPCG);

        }



// debug:
//        std::cout << "WIDataPCG DATA " << std::endl;
//        std::cout << "WIDataPCG.IJK " << WIDataPCG.IJK << std::endl;
//        std::cout << "WIDataPCG.WIC " << WIDataPCG.WCF << std::endl;

//        for( int ii=0; ii < in_fields.length(); ++ii ) {
//            std::cout << ii << ":" << in_fields[ii].toStdString() << std::endl;
//        }

//        COMPARE COMPDAT DATA
//
//        1. COMPARE NUMBER OF ROWS (NROWS)
//        	1.1	IF (NROWS) NOT EQUAL
//        	THEN ...
//        	ELSE CONTINUE TO STEP 2
//
//        2. COMPARE IJK COORDS
//	        2.1 IF (IJK) NOT EQUAL
//	        THEN: DETERMINE DEGREE OF DIFFERENCE
//	        ELSE: DO NOTHING, CONTINUE TO STEP 3
//
//        3. COMPARE WI VALUES



    }
}
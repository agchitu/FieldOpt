/******************************************************************************
   Created by Alin G. Chitu (2016-2019) <alin.chitu@tno.nl, chitu_alin@yahoo.com>
******************************************************************************/

#include "WellDefinition.h"
#include <fstream>
#include <map>
#include <iostream>

using namespace std;

namespace Reservoir {
namespace WellIndexCalculation {


void WellDefinition::ReadWellsFromFile(string file_path, vector<WellDefinition>& wells)
{
    ifstream infile(file_path);
    string previous_well_name = "";
    string well_name_delimiter = "->";
    string well_name;
    double hx, hy, hz, tx, ty, tz;
    double heel_md, toe_md;
    double radius;
    double skin_factor;
    
    std::string line;
    while (std::getline(infile, line))
    {
    	// Skip empty empty lines
    	if (line.find_first_not_of("\t\n\v\f\r") == std::string::npos || line.empty())
        {
    		continue;
    	}
    	
    	// Find well name
    	auto start_name_ind = line.find(well_name_delimiter);
    	if (start_name_ind!=std::string::npos)
        {
        	well_name = line.substr(0, start_name_ind);
        	start_name_ind += well_name_delimiter.length();
    	}
    	else
    	{       	
        	// Find the next tab
    		start_name_ind = line.find("\t");
    		well_name = line.substr(0, start_name_ind);
    		start_name_ind += 1;
    	}
   	
    	// Remove the well name
    	line.erase(0, start_name_ind);
    	
    	// Replace tabs with whitespaces
    	std::replace(line.begin(), line.end(), '\t', ' ');
        char* pAux;
  	    hx = strtod(line.c_str(), &pAux);
	    hy = strtod(pAux, &pAux);
	    hz = strtod(pAux, &pAux);
	    tx = strtod(pAux, &pAux);
	    ty = strtod(pAux, &pAux);
	    tz = strtod(pAux, &pAux);
            heel_md = strtod(pAux, &pAux);
            toe_md = strtod(pAux, &pAux);
	    radius  = strtod(pAux, &pAux);
	    skin_factor = strtod(pAux, &pAux);
	    
        if (previous_well_name != well_name) 
        {
            wells.push_back(WellDefinition());
            wells.back().wellname = well_name;
            previous_well_name = well_name;
        }

        wells.back().heels.push_back(Eigen::Vector3d(hx,hy,hz));
        wells.back().toes.push_back(Eigen::Vector3d(tx,ty,tz));
        wells.back().radii.push_back(radius);
        wells.back().skins.push_back(skin_factor);

	wells.back().heel_md.push_back(heel_md);
        wells.back().toe_md.push_back(toe_md);
    }
}
}
}


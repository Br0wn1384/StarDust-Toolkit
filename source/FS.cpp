/*
* StarDust
* Copyright (C) 2018  Team ReiSwitched
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <switch.h>
#include "FS.hpp"
#include "Net/Request.hpp"

using namespace std;

vector<string> FS::EnumDir(string path) {
    DIR *dp;
    vector<string> ret;
    struct dirent *ep;     
    dp = opendir (path.c_str());
    if (dp != NULL){
        while (ep = readdir(dp))
            ret.push_back(ep->d_name);
        closedir(dp);
    }
    return ret;
}

string FS::read_fl(string config_path) {
string devuelve = "";
    std::ifstream file(config_path.c_str());
    if (file.is_open()) {
        std::string line;
        getline(file, devuelve);
        file.close();
    }
	return devuelve;
}

string FS::getnettext(string nettext){
    Net net = Net();
    hidScanInput();
	net.readBuffer = "";
    string HBnew_release = "";
    HBnew_release = net.Request("GET",nettext);
	return net.readBuffer;
}

//check file exist
bool FS::check_ex(string check) {
ifstream file(check);
    if(file){
	file.close();
	return true;
	}else{
	file.close();
	return false;
	}
}
/*
* create flag function
*/
bool FS::Flag_me(string flags) {
    ofstream dest(flags, ios::binary);
    dest.close();
	return 0;
}

/*
* copy function
*/
bool FS::copy_me(string origen, string destino) {
    clock_t start, end;
    start = clock();
    ifstream source(origen, ios::binary);
    ofstream dest(destino, ios::binary);

    dest << source.rdbuf();

    source.close();
    dest.close();

    end = clock();

    cout << "CLOCKS_PER_SEC " << CLOCKS_PER_SEC << "\n";
    cout << "CPU-TIME START " << start << "\n";
    cout << "CPU-TIME END " << end << "\n";
    cout << "CPU-TIME END - START " <<  end - start << "\n";
    cout << "TIME(SEC) " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "\n";
	return 0;
}

bool FS::DirExists(string dirstr) {
    bool exist = false;
    DIR * dir = opendir(dirstr.c_str());
    if(dir) {
        closedir(dir);
        exist = true;
    }
    return exist;
}

unsigned FS::DeleteDirRecursive(string path) {
	int ret = -1;
	DIR * d = opendir(path.c_str());
	size_t path_len = strlen(path.c_str());
	if(d!=NULL) {
		struct dirent * de;
		ret = 0;
		
		while(!ret && (de=readdir(d))) {
			int status = -1;
			char * buf;
			size_t len;
			
			//skip over . and ..
			if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
				continue;
			}
			
			len = path_len + strlen(de->d_name);
			buf = (char *)malloc(len +2);
			
			if(buf!=NULL) {
				struct stat stator;
				snprintf(buf, len, "%s/%s", path.c_str(), de->d_name);
				
				if(!stat(buf, &stator)) {
					if(S_ISDIR(stator.st_mode)) {
						status = DeleteDirRecursive(buf);
					}
					else {
					status = unlink(buf);
					}
				}
				free(buf);
			}
			ret = status;
		}
		closedir(d);
	}
	
	if(!ret) {
		ret = rmdir(path.c_str());
	}
	
	return 0;
}

unsigned FS::MakeDir(string file, unsigned perm) {
    return mkdir(file.c_str(), perm);
}
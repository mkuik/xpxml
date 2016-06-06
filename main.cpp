/*
 * main.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: matthijs
 */

//#define DEBUG
#include "XMLParser.h"
#include "tools.h"
#include "Globals.h"
#include <csignal>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

const char * param[] {
        "//spectrum_query[position()<50]",
        "//spectrum_query[((@index>=4 and @start_scan!=@retention_time_sec) or /search_result/search_hit/@hit_rank<6) and (35 or 70)]/search_result/search_hit/@protein | /msms_pipeline_analysis/msms_run_summary/spectrum_query[5]/search_result/search_hit/@protein",
        "//spectrum_query[(@index>=4 and @start_scan!=@retention_time_sec) or /search_result/search_hit/@hit_rank<6]/search_result/search_hit/@protein",
        "/msms_pipeline_analysis/msms_run_summary/spectrum_query[(@index>=4 and @start_scan!=@retention_time_sec) and /search_result/search_hit/@hit_rank<6]/search_result/search_hit/@protein/text()",
        "/msms_pipeline_analysis//spectrum_query[@index<=10 and @start_scan>@retention_time_sec]/search_result/search_hit/@protein",
        "/msms_pipeline_analysis/msms_run_summary/spectrum_query[/search_result/search_hit/@hit_rank!=1]/search_result/search_hit/@protein",
        "/msms_pipeline_analysis/*/*[@index>4 and @start_scan!=@retention_time_sec]/@index",
        "/@*/string-length()",
        "spectrum_query//@peptide | spectrum_query//@protein",
        "spectrum_query[//@peptide/string-length()>10]/*",
        "@peptide | @protein",
        "@protein",
        "@peptide",
        "/msms_pipeline_analysis/msms_run_summary/spectrum_query/search_result/search_hit/@peptide/string-length()",
        "@*"};

void showUsageWarning(const char *tool) {
    // Inform the user of how to use the program
    std::cout << "Usage is " << tool << " -i <input> -x <xpath> [-o <output>][-xml][-max <id>][-h][-t <n>]\n";
    int i = 0;
    std::printf("\nxpath examples (-t)\n-----------------------\n");
    for (const char * xpath : param) {
        std::printf("%i\t%s\n", i, xpath);
        ++i;
    }
    exit(0);
}

std::streampos fileSize( const char* filePath ) {
    std::streampos fsize = 0;
    std::ifstream file( filePath, std::ios::binary );
    fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
    file.close();
    return fsize;
}

int main(const int argc, const char** argv) {

    const char * file[] {
    	    "./Data/10.pep.xml",
            "/home/matthijs/Dropbox/Studie/Hogeschool Leiden/Stage/Data/10.pep.xml",
            "/users/Matthijs/Dropbox/Studie/Hogeschool Leiden/Stage/Data/10.pep.xml",
            "/home/matthijs/Dropbox/Studie/Hogeschool Leiden/Stage/Data/Adult_Lung_Gel_Velos_13_f06.mzXML",
            "C:\\Users\\Matthijs Kuik\\Dropbox\\Studie\\Hogeschool Leiden\\Stage\\Data\\10.pep.xml",
            "/media/matthijs/Windows/Documents and Settings/Matthijs/Dropbox/Studie/Hogeschool Leiden/Stage/Data/10.pep.xml",
            "/storage/emulated/0/CppDroid/projects/xpxml/Data/10.pep.xml"};

    const char * IN = NULL;
    const char * PATH = NULL; // 3,4:error
    const char * OUT = NULL;
    XMLParser::Method METHOD = XMLParser::GET_VALUES;

    if (argc < 3) {
        showUsageWarning(argv[0]);
    } else { // if we got enough parameters...
        unsigned int *limit = 0;

        for (int i = 0; i != argc; i++) {
            if (strcmp(argv[i], "-xml") == 0) {
                METHOD = XMLParser::TO_XML;
            } else if (i + 1 != argc) {
                if (strcmp(argv[i], "-i") == 0) {
                    IN = argv[i + 1];
                } else if (strcmp(argv[i], "-o") == 0) {
                    OUT = argv[i + 1];
                } else if (strcmp(argv[i], "-x") == 0) {
                    PATH = argv[i + 1];
                } else if (strcmp(argv[i], "-max") == 0) {
                    limit = new unsigned int(
                            convert<unsigned int>(argv[i + 1]));
                } else if (strcmp(argv[i], "-t") == 0) {
                    try {
                        int index = convert<int>(argv[i + 1]);
                        PATH = param[index];
                    } catch (std::exception &e) {
                        std::printf("error: expected integer after -t option");
                        showUsageWarning(argv[0]);
                    }
                } else if (strcmp(argv[i], "-h") == 0) {
                    showUsageWarning(argv[0]);
                }
            }
        }

        if (IN == NULL) showUsageWarning(argv[0]);
        if (PATH == NULL) showUsageWarning(argv[0]);

        Globals::FILESIZE = fileSize(IN);

        ofstream of;
        try {
            XMLParser * parser = 0;
            if (OUT == NULL) {
                parser = new XMLParser(std::cout);
            } else {
                of.open(OUT, std::ios::out);
                parser = new XMLParser(of);
            }
            if (limit) {
                parser->setNodeIDLimit(*limit);
                delete limit;
            }
            parser->parse(IN, PATH, METHOD);
            delete parser;
        }
        catch (std::runtime_error e) {
            std::cout << e.what() << "\n";
        }
        of.close();

    }
    return 0;
}
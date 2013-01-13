
#include <stdio.h>

#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include "XUnzip.h"
#include <vector>
#include <stdint.h>

#include "pugixml/pugixml.hpp"
#include "checkDocument.h"

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 3) {
		puts(
			"Please specify filenames\n"
			"usage: xml_filename fla_filename\n"
		);
		return 0;
	}
	
	HZIP hz = OpenZip(argv[2], 0, ZIP_FILENAME);
	if (!hz) {
		puts("failed to open fla_filename.\n");
		return 0;
	}
	ZIPENTRYW entry;
	ZRESULT result;
	int index;
	const TCHAR* FILENAME = _T("PublishSettings.xml");
	result = FindZipItem(hz, FILENAME, true, &index, &entry);
	if (index == -1) {
		_tprintf(_T("failed to find %s in zip file.\n"), FILENAME);
		return 0;
	}
	std::vector<char> buff(entry.unc_size);
	result = UnzipItem(hz, entry.index, &buff[0], buff.size(), ZIP_MEMORY);
	CloseZip(hz);
	const char* pc = &buff[0];
	
	pugi::xml_parse_result parseResult;
	pugi::xml_document flaDoc;
	parseResult = flaDoc.load_buffer(pc, buff.size());
	if (!parseResult) {
		printf("failed to load xml buffer. %s\n", parseResult.description());
		return 0;
	}
	
	pugi::xml_document compareDoc;
	parseResult = compareDoc.load_file(argv[1]);
	
	checkDocument(flaDoc, compareDoc);
	
	return 0;
}


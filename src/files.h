#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "doomtype.h"

struct FileReader
{
public:
	FileReader (const char *filename);
	FileReader (FileReader &parent);
	FileReader (FileReader &parent, DWORD min, DWORD size);
	FileReader (int lumpnum);
	~FileReader ();

	bool IsOpen ();
	long GetLength ();
	long Tell ();
	long Seek (long offset, int origin);
	long Read (void *buffer, long len);

	template<class T> T *ReadAll ();

private:
	void *Handle;
	FileReader *Parent;

	DWORD MinPos;
	DWORD MaxPos;
	DWORD CurPos;
};

#endif

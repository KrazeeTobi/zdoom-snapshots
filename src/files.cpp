#include "files.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define WINIO
#endif

#ifndef WINIO
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BAD_HANDLE ((void*)-1)
#else
#define BAD_HANDLE ((void*)INVALID_HANDLE_VALUE)
#endif

FileReader::FileReader (const char *filename)
: Handle (BAD_HANDLE), Parent (NULL)
{
#ifdef WINIO
	HANDLE h = CreateFile (filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_RANDOM_ACCESS, NULL);

	if (h != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION info;

		if (GetFileInformationByHandle (h, &info))
		{
			MinPos = CurPos = 0;
			MaxPos = info.nFileSizeLow;
		}
		else
		{
			CloseHandle (h);
			h = INVALID_HANDLE_VALUE;
		}
	}
#else
	int h = open (filename, O_BINARY | O_RDONLY | O_RANDOM);

	if (h != -1)
	{
		long end;

		MinPos = CurPos = 0;
		end = lseek (h, 0, SEEK_END);
		if (end == 1)
		{
			close (h);
			h = -1;
		}
		else
		{
			MaxPos = (DWORD)end;
		}
	}
#endif
	Handle = (void*)h;
}

FileReader::FileReader (FileReader &parent)
: Handle (BAD_HANDLE)
{
	if (parent.Parent != NULL)
	{
		Parent = parent.Parent;
	}
	else
	{
		Parent = &parent;
	}
	MinPos = parent.MinPos;
	MaxPos = parent.MaxPos;
	CurPos = parent.MinPos;
}

FileReader::FileReader (FileReader &parent, DWORD min, DWORD size)
: Handle (BAD_HANDLE)
{
	if (parent.Parent != NULL)
	{
		Parent = parent.Parent;
	}
	else
	{
		Parent = &parent;
	}
	MinPos = parent.MinPos + min;
	MaxPos = MinPos + size;
	CurPos = parent.MinPos;
	if (MinPos >= parent.MaxPos || MaxPos > parent.MaxPos)
	{
		Parent = NULL;
	}
}

bool FileReader::IsOpen ()
{
	return Parent != NULL || Handle != BAD_HANDLE;
}

long FileReader::GetLength ()
{
	return MaxPos - MinPos;
}

long FileReader::Tell ()
{
	return CurPos - MinPos;
}

long FileReader::Seek (long offset, int origin)
{
	FileReader *master;
	DWORD pos;

	switch (origin)
	{
	case SEEK_SET:
		if (offset < 0)
		{
			return -1;
		}
		pos = offset + MinPos;
		if (pos > MaxPos)
		{
			return -1;
		}
		break;

	case SEEK_CUR:
		pos = offset + CurPos;
		if (pos < MinPos || pos > MaxPos)
		{
			return -1;
		}
		break;

	case SEEK_END:
		pos = offset + MaxPos;
		if (pos < MinPos || pos > MaxPos)
		{
			return -1;
		}
		break;

	default:
		return -1;
	}

	master = (Parent == NULL) ? this : Parent;

	CurPos = offset;

	if (master->CurPos != offset)
	{
#ifdef WINIO
		DWORD newPos = SetFilePointer ((HANDLE)master->Handle, pos, NULL, FILE_BEGIN);
		if (newPos == INVALID_SET_FILE_POINTER)
		{
			return -1;
		}
#else
		long newPos = lseek ((int)master->Handle, pos, SEEK_SET);
		if (newPos == -1)
		{
			return -1;
		}
#endif
		master->CurPos = newPos;
	}
	return offset - MinPos;
}

long FileReader::Read (void *buffer, long len)
{
	FileReader *master;

	if (Parent != NULL)
	{
		if (Parent->CurPos != CurPos)
		{
#ifdef WINIO
			if (INVALID_SET_FILE_POINTER == SetFilePointer ((HANDLE)Parent->Handle, CurPos, NULL, FILE_BEGIN))
			{
				return -1;
			}
#else
			if (-1 == lseek ((int)Parent->Handle, CurPos, SEEK_SET))
			{
				return -1;
			}
#endif
		}
		master = Parent;
	}
	else
	{
		master = this;
	}

	if (CurPos + len > MaxPos)
	{
		len = MaxPos - CurPos;
	}

#ifdef WINIO
	DWORD bytesRead;
	if (ReadFile ((HANDLE)master->Handle, buffer, len, &bytesRead, NULL))
	{
		return bytesRead;
	}
	else
	{
		return -1;
	}
#else
	return read ((int)master->Handle, buffer, len);
#endif
}

template<class T> T *FileReader::ReadAll ()
{
	DWORD numElems = (MaxPos - MinPos) / sizeof(T);
	T *stuff = new T[numElems];
	Seek (0, SEEK_SET);
	Read (stuff, numElems * sizeof(T));
}

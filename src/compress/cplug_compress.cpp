#include "cplug_compress.h"
#include "../cplug_filedir.h"
#include <vector>
#include "../log/cplug_log.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <zlib.h>
#include <zip.h>
#include <unzip.h>
#include <string.h>
#include "../cplug_str.h"

#define WRITEBUFFERSIZE (16384)
#define READBUFFERSIZE 8192
#define MAXFILENAME (256)
#define DIR_DELIMTER '/'

int isLargeFile(std::string filename)
{
	int largeFile = 0;
	ZPOS64_T pos = 0;
	FILE* pFile = NULL;
#ifdef _WIN32
	fopen_s(&pFile, filename.c_str(), "rb");
#else
	pFile = fopen(filename.c_str(), "rb");
#endif
	if (pFile != NULL)
	{
		int n = fseeko64(pFile, 0, SEEK_END);
		pos = ftello64(pFile);
		if (pos >= 0xffffffff)
			largeFile = 1;

		fclose(pFile);
	}

	return largeFile;
}

#ifdef _WIN32
uLong filetime(const char* f, tm_zip* tmzip, unsigned long* dt)
{
	int ret = 0;
	{
		FILETIME ftLocal;
		HANDLE hFind;
		WIN32_FIND_DATAA ff32;

		hFind = FindFirstFileA(f, &ff32);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftLocal);
			FileTimeToDosDateTime(&ftLocal, ((LPWORD)dt) + 1, ((LPWORD)dt) + 0);
			FindClose(hFind);
			ret = 1;
		}
	}
	return ret;
}
#elif unix || __APPLE__
#include <sys/stat.h>
uLong filetime(const char* f, tm_zip* tmzip, unsigned long* dt)
{
	int ret = 0;
	struct stat s;        /* results of stat() */
	struct tm* filedate;
	time_t tm_t = 0;

	if (strcmp(f, "-") != 0)
	{
		char name[MAXFILENAME + 1];
		int len = strlen(f);
		if (len > MAXFILENAME)
			len = MAXFILENAME;

		strncpy(name, f, MAXFILENAME - 1);
		/* strncpy doesnt append the trailing NULL, of the string is too long. */
		name[MAXFILENAME] = '\0';

		if (name[len - 1] == '/')
			name[len - 1] = '\0';
		/* not all systems allow stat'ing a file with / appended */
		if (stat(name, &s) == 0)
		{
			tm_t = s.st_mtime;
			ret = 1;
		}
	}
	filedate = localtime(&tm_t);

	tmzip->tm_sec = filedate->tm_sec;
	tmzip->tm_min = filedate->tm_min;
	tmzip->tm_hour = filedate->tm_hour;
	tmzip->tm_mday = filedate->tm_mday;
	tmzip->tm_mon = filedate->tm_mon;
	tmzip->tm_year = filedate->tm_year;

	return ret;
}
#else
uLong filetime(const char* f, tm_zip* tmzip, unsigned long* dt)
{
	return 0;
}
#endif

int Cplug_CP_ZipFile(const string& sZipFileName, const vector<string>& vUnZipFile, bool bAppendFile, int nCompressLevel)
{
	int size_buf = WRITEBUFFERSIZE;
	void* buf = NULL;
	buf = (void*)malloc(size_buf);

	if (buf == NULL)
	{
		logError("Error allocating memory");
		return ZIP_INTERNALERROR;
	}

	zipFile zf;
	int err, errclose;
	zf = zipOpen2_64(sZipFileName.c_str(), (bAppendFile) ? 2 : 0, NULL, NULL);
	//zf = zipOpen2(sZipFileName.c_str(), (bAppendFile) ? 2 : 0, NULL, NULL);
	//zf = zipOpen(Cplug_Str_StrToWstr(sZipFileName).c_str(), APPEND_STATUS_CREATE);

	if (zf == NULL)
	{
		logError("error opening %s", sZipFileName.c_str());
		err = ZIP_ERRNO;
	}
	else
	{
		err = ZIP_OK;
	}

	int i = 0;
	for (; i < vUnZipFile.size() && (err == ZIP_OK); i++)
	{
		FILE* fin = NULL;
		int size_read;
		std::string filenameinzip = vUnZipFile[i];
		std::string savefilenameinzip;
		zip_fileinfo zi;
		unsigned long crcFile = 0;
		int zip64 = 0;

		zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
			zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
		zi.dosDate = 0;
		zi.internal_fa = 0;
		zi.external_fa = 0;
		filetime(filenameinzip.c_str(), &zi.tmz_date, &zi.dosDate);

		zip64 = isLargeFile(filenameinzip);

		savefilenameinzip = Cplug_FD_GetFileNameFromFile(filenameinzip);

		err = zipOpenNewFileInZip3_64(zf, savefilenameinzip.c_str(), &zi,
			NULL, 0, NULL, 0, NULL /* comment*/,
			(nCompressLevel != 0) ? Z_DEFLATED : 0,
			nCompressLevel, 0,
			/* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
			-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
			NULL, crcFile, zip64);

		if (err != ZIP_OK)
			logError("error in opening %s in zipfile\n", filenameinzip.c_str());
		else
		{
#ifdef _WIN32
			fopen_s(&fin, filenameinzip.c_str(), "rb");
#else
			fin = fopen(filenameinzip.c_str(), "rb");
#endif

			if (fin == NULL)
			{
				err = ZIP_ERRNO;
				logError("error in opening %s for reading", filenameinzip.c_str());
			}
		}

		if (err == ZIP_OK)
		{
			do
			{
				err = ZIP_OK;
				size_read = (int)fread(buf, 1, size_buf, fin);
				if (size_read < size_buf)
					if (feof(fin) == 0)
					{
						logError("error in reading %s", filenameinzip.c_str());
						err = ZIP_ERRNO;
					}

				if (size_read > 0)
				{
					err = zipWriteInFileInZip(zf, buf, size_read);
					if (err < 0)
					{
						logError("error in writing %s in the zipfile",filenameinzip.c_str());
					}

				}
			} while ((err == ZIP_OK) && (size_read > 0));
		}

		if (fin)
			fclose(fin);

		if (err < 0)
			err = ZIP_ERRNO;
		else
		{
			err = zipCloseFileInZip(zf);
			if (err != ZIP_OK)
				logError("error in closing %s in the zipfile",filenameinzip.c_str());
		}
	}

	errclose = zipClose(zf, NULL);
	if (errclose != ZIP_OK)
		logError("error in closing %s", sZipFileName.c_str());

	if (buf != NULL)
		free(buf);

	return err;
}

int Cplug_CP_UnZipFile(const string& sZipFileName, const string& sUnZipFileDir)
{
	unzFile zipfile = unzOpen(sZipFileName.c_str());
	if (zipfile == NULL)
	{
		logError("%s: not found", sZipFileName.c_str());
		return UNZ_ERRNO;
	}

	unz_global_info64 global_info;
	if (unzGetGlobalInfo64(zipfile, &global_info) != UNZ_OK)
	{
		logError("[%s] could not read file global info", sZipFileName.c_str());
		unzClose(zipfile);
		return UNZ_ERRNO;
	}

	char read_buffer[100 * 1024];

	long i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info file_info;
		char filename[MAXFILENAME];
		if (unzGetCurrentFileInfo(
			zipfile,
			&file_info,
			filename,
			MAXFILENAME,
			NULL, 0, NULL, 0) != UNZ_OK)
		{
			logError("[%s] could not read file info", sZipFileName.c_str());
			unzClose(zipfile);
			return UNZ_ERRNO;
		}

		// Check if this entry is a directory or file.
		const size_t filename_length = strlen(filename);
		if (filename[filename_length - 1] == DIR_DELIMTER)
		{
			Cplug_FD_CreateDirectories(filename);
		}
		else
		{
			// Entry is a file, so extract it.
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				logError("could not open file [%s]", filename);
				unzClose(zipfile);
				return UNZ_ERRNO;
			}

			string sFullName = Cplug_FD_SpliceDirAndFileName(sUnZipFileDir, filename);

			Cplug_FD_CreateDirectories(Cplug_FD_GetDirFromFile(sFullName));
			FILE* out = fopen(sFullName.c_str(), "wb");
			if (out == NULL)
			{
				logError("could not open destination file [%s]", sFullName.c_str());
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return UNZ_ERRNO;
			}

			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, read_buffer, READBUFFERSIZE);
				if (error < 0)
				{
					logError("error read [%s], error code %d", sFullName.c_str(), error);
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return UNZ_ERRNO;
				}

				// Write data to file.
				if (error > 0)
				{
					fwrite(read_buffer, error, 1, out); // You should check return of fwrite...
				}
			} while (error > 0);

			fclose(out);
		}

		unzCloseCurrentFile(zipfile);

		// Go the the next entry listed in the zip file.
		if ((i + 1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				logError("cound not read next file");
				unzClose(zipfile);
				return UNZ_ERRNO;
			}
		}
	}

	unzClose(zipfile);
	return UNZ_OK;
}
